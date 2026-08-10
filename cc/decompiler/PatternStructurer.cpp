#include "Decompiler.h"

#include <algorithm>
#include <cstdlib>
#include <limits>
#include <map>
#include <queue>
#include <set>
#include <sstream>

namespace sango {
namespace decompiler {

namespace {

/** Stores the nodes and preferred exit of one natural loop. */
struct LoopRegion {
    int header = -1;
    int exit = -1;
    std::set<int> nodes;
    std::set<int> backEdgeSources;
};

/** Adds one indented source line to a structured function body. */
void addLine(std::vector<std::string>& output, int indent,
             const std::string& line) {
    const std::string prefix(static_cast<std::size_t>(indent), ' ');
    std::string indented = prefix;
    for (std::size_t index = 0; index < line.size(); ++index) {
        indented += line[index];
        if (line[index] == '\n' && index + 1 < line.size()) {
            indented += prefix;
        }
    }
    output.push_back(indented);
}

/** Returns a parenthesized logical inverse without assuming C precedence. */
std::string inverseCondition(const std::string& condition) {
    if (condition.size() >= 4 && condition.compare(0, 2, "!(") == 0
            && condition[condition.size() - 1] == ')') {
        int depth = 0;
        bool enclosesWholeExpression = true;
        for (std::size_t index = 1; index < condition.size(); ++index) {
            if (condition[index] == '(') {
                ++depth;
            } else if (condition[index] == ')') {
                --depth;
                if (depth == 0 && index + 1 != condition.size()) {
                    enclosesWholeExpression = false;
                    break;
                }
            }
        }
        if (enclosesWholeExpression && depth == 0) {
            return condition.substr(2, condition.size() - 3);
        }
    }
    return "!(" + condition + ")";
}

/** Computes shortest graph distances while optionally avoiding stop nodes. */
std::map<int, int> distancesFrom(const FunctionGraph& graph, int start,
                                 const std::set<int>& forbidden) {
    std::map<int, int> distance;
    std::queue<int> pending;
    if (start < 0 || forbidden.count(start) != 0) {
        return distance;
    }
    distance[start] = 0;
    pending.push(start);
    while (!pending.empty()) {
        const int block = pending.front();
        pending.pop();
        const std::vector<int>& successors = graph.blocks[block].successors;
        for (std::size_t index = 0; index < successors.size(); ++index) {
            const int successor = successors[index];
            if (forbidden.count(successor) == 0
                    && distance.find(successor) == distance.end()) {
                distance[successor] = distance[block] + 1;
                pending.push(successor);
            }
        }
    }
    return distance;
}

/** Finds the nearest block reachable from every supplied branch. */
int nearestCommonBlock(const FunctionGraph& graph,
                       const std::vector<int>& starts,
                       const std::set<int>& forbidden) {
    if (starts.empty()) {
        return -1;
    }
    std::vector<std::map<int, int> > allDistances;
    for (std::size_t index = 0; index < starts.size(); ++index) {
        allDistances.push_back(distancesFrom(graph, starts[index], forbidden));
    }
    int best = -1;
    int bestMaximum = std::numeric_limits<int>::max();
    int bestTotal = std::numeric_limits<int>::max();
    for (std::map<int, int>::const_iterator candidate = allDistances[0].begin();
            candidate != allDistances[0].end(); ++candidate) {
        int maximum = candidate->second;
        int total = candidate->second;
        bool common = true;
        for (std::size_t path = 1; path < allDistances.size(); ++path) {
            std::map<int, int>::const_iterator found
                = allDistances[path].find(candidate->first);
            if (found == allDistances[path].end()) {
                common = false;
                break;
            }
            maximum = std::max(maximum, found->second);
            total += found->second;
        }
        if (common && (maximum < bestMaximum
                || (maximum == bestMaximum && total < bestTotal))) {
            best = candidate->first;
            bestMaximum = maximum;
            bestTotal = total;
        }
    }
    return best;
}

/** Finds the nearest join shared by at least two non-terminating branches. */
int nearestSharedBlock(const FunctionGraph& graph,
                       const std::vector<int>& starts,
                       const std::set<int>& forbidden) {
    if (starts.size() < 2) {
        return -1;
    }
    std::vector<std::map<int, int> > allDistances;
    std::set<int> candidates;
    std::set<int> branchStarts(starts.begin(), starts.end());
    for (std::size_t index = 0; index < starts.size(); ++index) {
        allDistances.push_back(distancesFrom(graph, starts[index], forbidden));
        for (std::map<int, int>::const_iterator found
                = allDistances.back().begin();
                found != allDistances.back().end(); ++found) {
            if (branchStarts.count(found->first) == 0) {
                candidates.insert(found->first);
            }
        }
    }

    int best = -1;
    int bestCoverage = 1;
    int bestMaximum = std::numeric_limits<int>::max();
    int bestTotal = std::numeric_limits<int>::max();
    for (std::set<int>::const_iterator candidate = candidates.begin();
            candidate != candidates.end(); ++candidate) {
        int coverage = 0;
        int maximum = 0;
        int total = 0;
        for (std::size_t path = 0; path < allDistances.size(); ++path) {
            std::map<int, int>::const_iterator found
                = allDistances[path].find(*candidate);
            if (found == allDistances[path].end()) {
                continue;
            }
            ++coverage;
            maximum = std::max(maximum, found->second);
            total += found->second;
        }
        if (coverage > bestCoverage
                || (coverage == bestCoverage && maximum < bestMaximum)
                || (coverage == bestCoverage && maximum == bestMaximum
                    && total < bestTotal)) {
            best = *candidate;
            bestCoverage = coverage;
            bestMaximum = maximum;
            bestTotal = total;
        }
    }
    return bestCoverage >= 2 ? best : -1;
}

/** Discovers natural loops from backward edges in compiler-ordered blocks. */
std::map<int, LoopRegion> findLoops(const FunctionGraph& graph) {
    std::map<int, LoopRegion> loops;
    for (std::size_t source = 0; source < graph.blocks.size(); ++source) {
        const std::vector<int>& successors = graph.blocks[source].successors;
        for (std::size_t edge = 0; edge < successors.size(); ++edge) {
            const int header = successors[edge];
            if (header > static_cast<int>(source)) {
                continue;
            }
            LoopRegion& loop = loops[header];
            loop.header = header;
            loop.nodes.insert(header);
            loop.nodes.insert(static_cast<int>(source));
            loop.backEdgeSources.insert(static_cast<int>(source));
            std::vector<int> pending(1, static_cast<int>(source));
            while (!pending.empty()) {
                const int node = pending.back();
                pending.pop_back();
                const std::vector<int>& predecessors = graph.blocks[node].predecessors;
                for (std::size_t predecessor = 0;
                        predecessor < predecessors.size(); ++predecessor) {
                    const int value = predecessors[predecessor];
                    if (value >= header && loop.nodes.insert(value).second
                            && value != header) {
                        pending.push_back(value);
                    }
                }
            }
        }
    }
    for (std::map<int, LoopRegion>::iterator loop = loops.begin();
            loop != loops.end(); ++loop) {
        std::set<int> exits;
        for (std::set<int>::const_iterator node = loop->second.nodes.begin();
                node != loop->second.nodes.end(); ++node) {
            const std::vector<int>& successors = graph.blocks[*node].successors;
            for (std::size_t edge = 0; edge < successors.size(); ++edge) {
                if (loop->second.nodes.count(successors[edge]) == 0) {
                    exits.insert(successors[edge]);
                }
            }
        }
        if (!exits.empty()) {
            std::vector<int> exitStarts(exits.begin(), exits.end());
            loop->second.exit = nearestCommonBlock(
                graph, exitStarts, loop->second.nodes);
            std::set<int> headerExits;
            const std::vector<int>& headerSuccessors
                = graph.blocks[loop->second.header].successors;
            for (std::size_t edge = 0; edge < headerSuccessors.size(); ++edge) {
                if (loop->second.nodes.count(headerSuccessors[edge]) == 0) {
                    headerExits.insert(headerSuccessors[edge]);
                }
            }
            if (loop->second.exit < 0 && headerExits.size() == 1) {
                loop->second.exit = *headerExits.begin();
            }
            std::set<int> latchExits;
            for (std::set<int>::const_iterator source
                    = loop->second.backEdgeSources.begin();
                    source != loop->second.backEdgeSources.end(); ++source) {
                const std::vector<int>& successors
                    = graph.blocks[*source].successors;
                for (std::size_t edge = 0; edge < successors.size(); ++edge) {
                    if (loop->second.nodes.count(successors[edge]) == 0) {
                        latchExits.insert(successors[edge]);
                    }
                }
            }
            if (loop->second.exit < 0 && latchExits.size() == 1) {
                loop->second.exit = *latchExits.begin();
            }
            if (loop->second.exit < 0) {
                loop->second.exit = *exits.begin();
            }
        }
    }
    return loops;
}

/** Emits structured regions while retaining a goto for genuinely shared flow. */
class RegionWriter {
public:
    /** Prepares a writer for one graph and its corresponding lifted blocks. */
    RegionWriter(const FunctionGraph& graph, LiftedFunction& function)
        : graph_(graph), function_(function), loops_(findLoops(graph)) {}

    /** Emits the complete entry region and any residual unreachable blocks. */
    void write() {
        std::set<int> noStops;
        emitRegion(0, noStops, NULL, 0);
        for (std::size_t index = 0; index < graph_.blocks.size(); ++index) {
            if (emitted_.count(static_cast<int>(index)) == 0) {
                emitRegion(static_cast<int>(index), noStops, NULL, 0);
            }
        }
        std::vector<std::string> finalized;
        for (std::size_t index = 0; index < function_.structuredBody.size(); ++index) {
            const std::string& line = function_.structuredBody[index];
            if (line.compare(0, 8, "@block: ") == 0) {
                const int block = std::atoi(line.c_str() + 8);
                if (labelTargets_.count(block) != 0) {
                    finalized.push_back(label(block) + ":");
                }
            } else {
                finalized.push_back(line);
            }
        }
        function_.structuredBody.swap(finalized);
    }

private:
    const FunctionGraph& graph_;
    LiftedFunction& function_;
    std::map<int, LoopRegion> loops_;
    std::set<int> emitted_;
    std::set<int> labelTargets_;

    /** Returns a globally unique source label for a basic block. */
    std::string label(int block) const {
        return "function_" + std::to_string(function_.functionIndex)
            + "_block_" + std::to_string(block);
    }

    /** Emits the non-control statements stored in one lifted block. */
    void emitStatements(int block, int indent) {
        const std::vector<std::string>& statements
            = function_.blocks[block].statements;
        for (std::size_t index = 0; index < statements.size(); ++index) {
            addLine(function_.structuredBody, indent, statements[index]);
        }
    }

    /** Emits a residual jump and records it in readability metrics. */
    void emitGoto(int target, int indent) {
        labelTargets_.insert(target);
        addLine(function_.structuredBody, indent, "goto " + label(target) + ";");
        ++function_.residualGotoCount;
    }

    /** Collects a straight-line path from a loop branch to its common exit. */
    bool collectLoopExitPath(int start, const LoopRegion& loop,
                             std::vector<int>& path) const {
        path.clear();
        std::set<int> visited;
        int current = start;
        while (current >= 0 && loop.nodes.count(current) == 0
                && visited.insert(current).second) {
            if (current == loop.exit) {
                return true;
            }
            const LiftedBlock& block = function_.blocks[current];
            if (emitted_.count(current) != 0) {
                return false;
            }
            path.push_back(current);
            if (block.exitKind == ExitKind::jump) {
                current = block.target;
            } else if (block.exitKind == ExitKind::fallthrough) {
                current = block.fallthrough;
            } else {
                return false;
            }
        }
        return false;
    }

    /** Emits the statements collected on a straight-line loop exit path. */
    void emitLoopExitPath(const std::vector<int>& path, int indent) {
        for (std::size_t index = 0; index < path.size(); ++index) {
            emitted_.insert(path[index]);
            function_.structuredBody.push_back(
                "@block: " + std::to_string(path[index]));
            emitStatements(path[index], indent);
        }
    }

    /** Emits a conditional loop exit, including any cleanup before its break. */
    void emitConditionalLoopExit(const std::string& condition,
                                 const std::vector<int>& path, int indent) {
        if (path.empty()) {
            addLine(function_.structuredBody, indent,
                    "if (" + condition + ") break;");
            return;
        }
        addLine(function_.structuredBody, indent,
                "if (" + condition + ") {");
        emitLoopExitPath(path, indent + 4);
        addLine(function_.structuredBody, indent + 4, "break;");
        addLine(function_.structuredBody, indent, "}");
    }

    /** Emits a return block reached only by the current loop branch. */
    bool emitDedicatedReturn(int blockIndex, int indent) {
        if (blockIndex < 0 || emitted_.count(blockIndex) != 0
                || graph_.blocks[blockIndex].predecessors.size() != 1) {
            return false;
        }
        const LiftedBlock& block = function_.blocks[blockIndex];
        if (block.exitKind != ExitKind::returnWithoutValue
                && block.exitKind != ExitKind::returnWithValue) {
            return false;
        }
        emitted_.insert(blockIndex);
        function_.structuredBody.push_back(
            "@block: " + std::to_string(blockIndex));
        emitStatements(blockIndex, indent);
        if (block.exitKind == ExitKind::returnWithValue) {
            addLine(function_.structuredBody, indent,
                    "return " + block.returnValue + ";");
        } else {
            addLine(function_.structuredBody, indent, "return;");
        }
        return true;
    }

    /** Emits a structured branch body that leaves a loop without re-entering it. */
    bool emitStructuredLoopDeparture(int start, const LoopRegion& loop,
                                     int indent) {
        const int previousGotoCount = function_.residualGotoCount;
        std::set<int> stops;
        stops.insert(loop.exit);
        const int reached = emitRegion(start, stops, NULL, indent);
        if (reached == loop.exit) {
            addLine(function_.structuredBody, indent, "break;");
        }
        return function_.residualGotoCount == previousGotoCount
            && (reached == loop.exit || reached < 0);
    }

    /** Emits a loop body and resumes at the loop's preferred exit. */
    int emitLoop(const LoopRegion& loop, int indent) {
        const LiftedBlock& header = function_.blocks[loop.header];
        if (header.statements.empty() && header.exitKind == ExitKind::branch) {
            const bool trueInside = loop.nodes.count(header.target) != 0;
            const bool falseInside = loop.nodes.count(header.fallthrough) != 0;
            const int outside = trueInside
                ? header.fallthrough : header.target;
            if (trueInside != falseInside && outside == loop.exit) {
                const int body = trueInside ? header.target : header.fallthrough;
                const std::string condition = trueInside
                    ? header.condition : inverseCondition(header.condition);
                emitted_.insert(loop.header);
                function_.structuredBody.push_back(
                    "@block: " + std::to_string(loop.header));
                addLine(function_.structuredBody, indent,
                        "while (" + condition + ") {");
                std::set<int> stops;
                stops.insert(loop.header);
                emitRegion(body, stops, &loop, indent + 4);
                addLine(function_.structuredBody, indent, "}");
                return loop.exit;
            }
        }
        addLine(function_.structuredBody, indent, "while (1) {");
        std::set<int> stops;
        stops.insert(loop.header);
        emitRegion(loop.header, stops, &loop, indent + 4, true);
        addLine(function_.structuredBody, indent, "}");
        return loop.exit;
    }

    /** Emits one compiler switch chain when all dispatch blocks are recognizable. */
    bool emitSwitch(int firstBlock, const LoopRegion* activeLoop,
                    int indent, int& nextBlock) {
        std::vector<std::pair<std::int32_t, int> > cases;
        std::set<int> dispatchBlocks;
        int dispatch = firstBlock;
        const std::string selector = function_.blocks[firstBlock].condition;
        while (dispatch >= 0
                && function_.blocks[dispatch].exitKind == ExitKind::switchCase
                && function_.blocks[dispatch].condition == selector
                && dispatchBlocks.insert(dispatch).second) {
            cases.push_back(std::make_pair(
                function_.blocks[dispatch].switchValue,
                function_.blocks[dispatch].target));
            dispatch = function_.blocks[dispatch].fallthrough;
        }
        if (cases.empty() || dispatch < 0) {
            return false;
        }
        const LiftedBlock& tail = function_.blocks[dispatch];
        const bool discardsRetainedSelector
            = tail.statements.size() == 1
            && tail.statements[0] == selector + ";";
        if ((!tail.statements.empty() && !discardsRetainedSelector)
                || tail.exitKind != ExitKind::jump || tail.target < 0) {
            return false;
        }
        const int defaultTarget = tail.target;
        std::vector<int> branchStarts;
        for (std::size_t index = 0; index < cases.size(); ++index) {
            branchStarts.push_back(cases[index].second);
        }
        branchStarts.push_back(defaultTarget);
        std::set<int> forbidden = dispatchBlocks;
        forbidden.insert(dispatch);
        int join = nearestCommonBlock(graph_, branchStarts, forbidden);
        if (join < 0) {
            join = nearestSharedBlock(graph_, branchStarts, forbidden);
        }
        if (join < 0) {
            emitted_.insert(dispatchBlocks.begin(), dispatchBlocks.end());
            emitted_.insert(dispatch);
            function_.keepsSwitchWithGotos = true;
            addLine(function_.structuredBody, indent,
                    "switch (" + selector + ") {");
            std::set<int> boundaries(branchStarts.begin(), branchStarts.end());
            for (std::size_t index = 0; index < cases.size(); ++index) {
                addLine(function_.structuredBody, indent + 4,
                    "case " + std::to_string(cases[index].first) + ":");
                std::set<int> stops = boundaries;
                stops.erase(cases[index].second);
                const int reached = emitRegion(cases[index].second, stops,
                                               activeLoop, indent + 8);
                const bool fallsIntoNextCase
                    = index + 1 < cases.size()
                    && reached == cases[index + 1].second;
                const bool alreadyContinuesLoop = activeLoop != NULL
                    && reached == activeLoop->header;
                if (reached >= 0 && !fallsIntoNextCase
                        && !alreadyContinuesLoop) {
                    emitGoto(reached, indent + 8);
                }
            }
            addLine(function_.structuredBody, indent + 4, "default:");
            std::set<int> defaultStops = boundaries;
            defaultStops.erase(defaultTarget);
            const int defaultReached = emitRegion(
                defaultTarget, defaultStops, activeLoop, indent + 8);
            const bool defaultContinuesLoop = activeLoop != NULL
                && defaultReached == activeLoop->header;
            if (defaultReached >= 0 && !defaultContinuesLoop) {
                emitGoto(defaultReached, indent + 8);
            }
            addLine(function_.structuredBody, indent, "}");
            nextBlock = -1;
            return true;
        }

        emitted_.insert(dispatchBlocks.begin(), dispatchBlocks.end());
        emitted_.insert(dispatch);
        addLine(function_.structuredBody, indent, "switch (" + selector + ") {");
        std::set<int> boundaries;
        for (std::size_t index = 0; index < branchStarts.size(); ++index) {
            boundaries.insert(branchStarts[index]);
        }
        boundaries.insert(join);

        for (std::size_t index = 0; index < cases.size(); ++index) {
            addLine(function_.structuredBody, indent + 4,
                "case " + std::to_string(cases[index].first) + ":");
            std::set<int> stops = boundaries;
            stops.erase(cases[index].second);
            const int reached = emitRegion(cases[index].second, stops,
                                           activeLoop, indent + 8);
            const bool fallsIntoNextCase = index + 1 < cases.size()
                && reached == cases[index + 1].second;
            const bool fallsIntoDefault = defaultTarget != join
                && index + 1 == cases.size() && reached == defaultTarget;
            const bool continuesLoop = activeLoop != NULL
                && reached == activeLoop->header;
            if (reached == join) {
                addLine(function_.structuredBody, indent + 8, "break;");
            } else if (reached >= 0 && !fallsIntoNextCase
                    && !fallsIntoDefault && !continuesLoop) {
                emitGoto(reached, indent + 8);
            }
        }
        if (defaultTarget != join) {
            addLine(function_.structuredBody, indent + 4, "default:");
            std::set<int> defaultStops = boundaries;
            defaultStops.erase(defaultTarget);
            const int defaultReached = emitRegion(defaultTarget, defaultStops,
                                                  activeLoop, indent + 8);
            const bool defaultContinuesLoop = activeLoop != NULL
                && defaultReached == activeLoop->header;
            if (defaultReached == join) {
                addLine(function_.structuredBody, indent + 8, "break;");
            } else if (defaultReached >= 0 && !defaultContinuesLoop) {
                emitGoto(defaultReached, indent + 8);
            }
        }
        addLine(function_.structuredBody, indent, "}");
        nextBlock = join;
        return true;
    }

    /** Emits one region until it terminates or reaches a caller-supplied stop. */
    int emitRegion(int start, const std::set<int>& stops,
                   const LoopRegion* activeLoop, int indent,
                   bool allowLoopHeader = false) {
        int current = start;
        while (current >= 0) {
            if (stops.count(current) != 0 && !allowLoopHeader) {
                return current;
            }
            allowLoopHeader = false;
            if (emitted_.count(current) != 0) {
                emitGoto(current, indent);
                return -1;
            }
            std::map<int, LoopRegion>::const_iterator nested = loops_.find(current);
            if (nested != loops_.end()
                    && (activeLoop == NULL || activeLoop->header != current)) {
                current = emitLoop(nested->second, indent);
                continue;
            }

            emitted_.insert(current);
            function_.structuredBody.push_back("@block: " + std::to_string(current));
            const LiftedBlock& block = function_.blocks[current];
            emitStatements(current, indent);

            if (block.exitKind == ExitKind::returnWithoutValue) {
                addLine(function_.structuredBody, indent, "return;");
                return -1;
            }
            if (block.exitKind == ExitKind::returnWithValue) {
                addLine(function_.structuredBody, indent,
                        "return " + block.returnValue + ";");
                return -1;
            }
            if (block.exitKind == ExitKind::switchCase) {
                int afterSwitch = -1;
                if (emitSwitch(current, activeLoop, indent, afterSwitch)) {
                    current = afterSwitch;
                    continue;
                }
            }
            if (block.exitKind == ExitKind::fallthrough) {
                current = block.fallthrough;
                continue;
            }
            if (block.exitKind == ExitKind::jump) {
                if (stops.count(block.target) != 0) {
                    return block.target;
                }
                if (activeLoop != NULL && block.target == activeLoop->header) {
                    addLine(function_.structuredBody, indent, "continue;");
                    return block.target;
                }
                if (activeLoop != NULL
                        && activeLoop->nodes.count(block.target) == 0) {
                    std::vector<int> exitPath;
                    if (collectLoopExitPath(
                            block.target, *activeLoop, exitPath)) {
                        emitLoopExitPath(exitPath, indent);
                        addLine(function_.structuredBody, indent, "break;");
                    } else {
                        emitGoto(block.target, indent);
                    }
                    return block.target;
                }
                current = block.target;
                continue;
            }
            if (block.exitKind == ExitKind::branch) {
                const int whenTrue = block.target;
                const int whenFalse = block.fallthrough;
                if (activeLoop != NULL) {
                    const bool trueContinues = whenTrue == activeLoop->header;
                    const bool falseContinues = whenFalse == activeLoop->header;
                    const bool trueBreaks = activeLoop->nodes.count(whenTrue) == 0;
                    const bool falseBreaks = activeLoop->nodes.count(whenFalse) == 0;
                    if (trueContinues || falseContinues) {
                        const int other = trueContinues ? whenFalse : whenTrue;
                        std::vector<int> exitPath;
                        if (activeLoop->nodes.count(other) == 0
                                && collectLoopExitPath(
                                    other, *activeLoop, exitPath)) {
                            emitConditionalLoopExit(trueContinues
                                ? inverseCondition(block.condition)
                                : block.condition, exitPath, indent);
                            return activeLoop->header;
                        }
                        addLine(function_.structuredBody, indent,
                            "if (" + (trueContinues ? block.condition
                                                    : inverseCondition(block.condition))
                            + ") continue;");
                        current = trueContinues ? whenFalse : whenTrue;
                        continue;
                    }
                    if (trueBreaks || falseBreaks) {
                        const int outside = trueBreaks ? whenTrue : whenFalse;
                        const int inside = trueBreaks ? whenFalse : whenTrue;
                        const std::string exitCondition = trueBreaks
                            ? block.condition : inverseCondition(block.condition);
                        std::vector<int> exitPath;
                        const bool reachesExit = collectLoopExitPath(
                            outside, *activeLoop, exitPath);
                        if (reachesExit) {
                            emitConditionalLoopExit(
                                exitCondition, exitPath, indent);
                        } else {
                            addLine(function_.structuredBody, indent,
                                    "if (" + exitCondition + ") {");
                            if (!emitDedicatedReturn(outside, indent + 4)
                                    && !emitStructuredLoopDeparture(
                                        outside, *activeLoop, indent + 4)) {
                                emitGoto(outside, indent + 4);
                            }
                            addLine(function_.structuredBody, indent, "}");
                        }
                        if (!reachesExit
                                && emitted_.count(outside) == 0) {
                            labelTargets_.insert(outside);
                        }
                        current = inside;
                        continue;
                    }
                }
                if (stops.count(whenTrue) != 0 || stops.count(whenFalse) != 0) {
                    const bool trueStops = stops.count(whenTrue) != 0;
                    const int body = trueStops ? whenFalse : whenTrue;
                    addLine(function_.structuredBody, indent,
                        "if (" + (trueStops ? inverseCondition(block.condition)
                                              : block.condition) + ") {");
                    emitRegion(body, stops, activeLoop, indent + 4);
                    addLine(function_.structuredBody, indent, "}");
                    return trueStops ? whenTrue : whenFalse;
                }

                std::vector<int> starts;
                starts.push_back(whenTrue);
                starts.push_back(whenFalse);
                std::set<int> forbidden;
                if (activeLoop != NULL) {
                    forbidden.insert(activeLoop->header);
                }
                const int join = nearestCommonBlock(graph_, starts, forbidden);
                if (join >= 0 && join != current) {
                    std::set<int> branchStops = stops;
                    branchStops.insert(join);
                    if (whenTrue == join || whenFalse == join) {
                        const bool trueIsEmpty = whenTrue == join;
                        const int body = trueIsEmpty ? whenFalse : whenTrue;
                        addLine(function_.structuredBody, indent,
                                "if (" + (trueIsEmpty
                                    ? inverseCondition(block.condition)
                                    : block.condition) + ") {");
                        emitRegion(body, branchStops, activeLoop, indent + 4);
                        addLine(function_.structuredBody, indent, "}");
                        current = join;
                        continue;
                    }
                    addLine(function_.structuredBody, indent,
                            "if (" + block.condition + ") {");
                    emitRegion(whenTrue, branchStops, activeLoop, indent + 4);
                    addLine(function_.structuredBody, indent, "} else {");
                    emitRegion(whenFalse, branchStops, activeLoop, indent + 4);
                    addLine(function_.structuredBody, indent, "}");
                    current = join;
                    continue;
                }

                addLine(function_.structuredBody, indent,
                        "if (" + block.condition + ") {");
                emitRegion(whenTrue, stops, activeLoop, indent + 4);
                addLine(function_.structuredBody, indent, "} else {");
                emitRegion(whenFalse, stops, activeLoop, indent + 4);
                addLine(function_.structuredBody, indent, "}");
                return -1;
            }

            emitGoto(block.target, indent);
            return -1;
        }
        return -1;
    }
};

/** Replaces residual jumps with a local-state dispatcher for irreducible flow. */
void buildDispatcher(LiftedFunction& function) {
    function.structuredBody.clear();
    const std::string state = "control_state_"
        + std::to_string(function.functionIndex);
    addLine(function.structuredBody, 0, state + " = 0;");
    addLine(function.structuredBody, 0, "while (1) {");
    addLine(function.structuredBody, 4, "switch (" + state + ") {");
    for (std::size_t index = 0; index < function.blocks.size(); ++index) {
        const LiftedBlock& block = function.blocks[index];
        addLine(function.structuredBody, 8,
                "case " + std::to_string(index) + ":");
        for (std::size_t statement = 0;
                statement < block.statements.size(); ++statement) {
            addLine(function.structuredBody, 12, block.statements[statement]);
        }
        if (block.exitKind == ExitKind::returnWithoutValue) {
            addLine(function.structuredBody, 12, "return;");
        } else if (block.exitKind == ExitKind::returnWithValue) {
            addLine(function.structuredBody, 12,
                    "return " + block.returnValue + ";");
        } else if (block.exitKind == ExitKind::branch) {
            addLine(function.structuredBody, 12, "if (" + block.condition + ") {");
            addLine(function.structuredBody, 16,
                    state + " = " + std::to_string(block.target) + ";");
            addLine(function.structuredBody, 12, "} else {");
            addLine(function.structuredBody, 16,
                    state + " = " + std::to_string(block.fallthrough) + ";");
            addLine(function.structuredBody, 12, "}");
            addLine(function.structuredBody, 12, "continue;");
        } else if (block.exitKind == ExitKind::switchCase) {
            addLine(function.structuredBody, 12,
                "if (" + block.condition + " == "
                + std::to_string(block.switchValue) + ") {");
            addLine(function.structuredBody, 16,
                    state + " = " + std::to_string(block.target) + ";");
            addLine(function.structuredBody, 12, "} else {");
            addLine(function.structuredBody, 16,
                    state + " = " + std::to_string(block.fallthrough) + ";");
            addLine(function.structuredBody, 12, "}");
            addLine(function.structuredBody, 12, "continue;");
        } else {
            const int target = block.exitKind == ExitKind::jump
                ? block.target : block.fallthrough;
            if (target >= 0) {
                addLine(function.structuredBody, 12,
                    state + " = " + std::to_string(target) + ";");
                addLine(function.structuredBody, 12, "continue;");
            } else if (function.returnType == ValueType::string) {
                addLine(function.structuredBody, 12, "return \"\";");
            } else if (function.returnType == ValueType::floatingPoint) {
                addLine(function.structuredBody, 12, "return 0.0;");
            } else if (!function.returnsValue) {
                addLine(function.structuredBody, 12, "return;");
            } else {
                addLine(function.structuredBody, 12, "return 0;");
            }
        }
    }
    addLine(function.structuredBody, 8, "default:");
    if (!function.returnsValue) {
        addLine(function.structuredBody, 12, "return;");
    } else if (function.returnType == ValueType::string) {
        addLine(function.structuredBody, 12, "return \"\";");
    } else if (function.returnType == ValueType::floatingPoint) {
        addLine(function.structuredBody, 12, "return 0.0;");
    } else {
        addLine(function.structuredBody, 12, "return 0;");
    }
    addLine(function.structuredBody, 4, "}");
    addLine(function.structuredBody, 0, "}");
    function.residualGotoCount = 0;
    function.usesDispatcher = true;
}

}  // namespace

void PatternStructurer::structure(const FunctionGraph& graph,
                                  LiftedFunction& function) const {
    function.structuredBody.clear();
    function.residualGotoCount = 0;
    function.structured = false;
    if (!function.succeeded || function.blocks.size() != graph.blocks.size()) {
        return;
    }
    RegionWriter writer(graph, function);
    writer.write();
    if (function.residualGotoCount != 0 && !function.keepsSwitchWithGotos) {
        buildDispatcher(function);
    }
    function.structured = true;
}

}  // namespace decompiler
}  // namespace sango
