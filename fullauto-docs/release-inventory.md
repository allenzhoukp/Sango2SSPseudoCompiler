# Delivery inventory

Reviewed 2026-07-19 against the packaged deliveries in `../release` and the
recovered local Git history. The ZIP archives are the authority for what was
actually shipped; tags described as a "source baseline" do not claim that
release-only binaries or every packaged auxiliary file are present in Git.

## Version map

| Version | Packaged ZIP | Local Git reference | Assessment |
| --- | --- | --- | --- |
| v0.1 | `Sango2SSPseudoComplier_v0.1.zip` | none | No commit contains a reliable source snapshot; archive-only delivery. |
| v0.2 | `Sango2SSPseudoComplier_v0.2.zip` | none | A few files match the first imported commits, but no commit represents the full source delivery. |
| v0.3 | `Sango2SSPseudoCompiler_v0.3.zip` | `v0.3` -> `0cc6a7d` | Exact packaged baseline: all 19 files match, allowing for CRLF normalization. |
| v0.4 | no ZIP in this directory | `v0.4` -> `ecda825` | Existing version tag/history milestone, not a reviewed packaged ZIP. |
| v0.9.0 | `Sango2SSPseudoComplier_v0.9.0.zip` | `v0.9.0` -> `cd00690` | Packaged source baseline; generated executables differ or are release-only. |
| v0.9.1 | no ZIP in this directory | `v0.9.1` -> `07cf1bc` | Version milestone identified by the commit and user guide. |
| v0.9.2 | `Sango2SSPseudoCompiler_v0.9.2.zip` | `v0.9.2` -> `7319301` | Packaged source baseline; the ZIP also contains release-only templates/binaries. |
| v0.9.3 | `Sango2SSPseudoCompiler_v0.9.3.zip` | `v0.9.3` -> `f60c07c` | Reliable source baseline: 32/33 files match; only `sg2asm.exe` is package-only. |
| v0.9.4 | `Sango2SSPseudoCompiler_v0.9.4.zip` | none | Composite/uncommitted delivery: files match both v0.9.3 and later v0.9.5 work, while the guide and `ss.exe` match no commit. |
| v0.9.5 | `Sango2SSPseudoCompiler_v0.9.5.zip` | none | Composite snapshot with files from multiple revisions and several files that match no commit; do not use the partial `v0.9.5 step one` commit as a release tag. |
| v0.9.6 | `Sango2SSPseudoCompiler_v0.9.6.zip` | `v0.9.6` -> `23a7f51` | Packaged source baseline; the ZIP has a later guide and release-only assembler. |
| v0.9.7 | `Sango2SSPseudoCompiler_v0.9.7.zip` | `v0.9.7` -> `c1a4783` | Best packaged baseline. All Git-available shipped files match after CRLF normalization; `sg2asm.exe` is release-only at this commit. |
| v0.9.8 | no ZIP in this directory | `v0.9.8` -> `9899a60` | Delivery baseline from the recovered Git history and v0.9.8 user guide. |

The v0.9.7 tag intentionally points to the Big5 hotfix commit `c1a4783`, not
the earlier `c6b981e` commit whose subject also says v0.9.7. The ZIP contains
the hotfix versions of the affected sources.

## Archive checksums

SHA-256:

```text
27b5572f678fdcabe4a9e24c5e1d27a0e2159a5e32607d814676f6952f57b6bc  Sango2SSPseudoComplier_v0.1.zip
2a2d8d59109ebe7b58099015668c1b021d1ecc6fc215f775039847ef600a1c44  Sango2SSPseudoComplier_v0.2.zip
d49d0bca2e91c9b8a644ad6050244dce66f57e294cecdc37f837f13f5efc93e5  Sango2SSPseudoCompiler_v0.3.zip
bdb9ef0878ae1c2956e01c387aa7d1bedfa61fc0d33735fa51238848e63f9211  Sango2SSPseudoComplier_v0.9.0.zip
0a7b622aca585224470987b43d20c1ea5720a2dc98d4efc394922ffaff515686  Sango2SSPseudoCompiler_v0.9.2.zip
d423674ba6be5e1fc4b71073a1fd43b2d45ae301a21860494347d67dfae01dc6  Sango2SSPseudoCompiler_v0.9.3.zip
87b4d52684d3fd510b946aa9e0d42433ab8a89ebcdb30e486194c1407aea6fa7  Sango2SSPseudoCompiler_v0.9.4.zip
febc95dda667c50a899097f1937ac3c106d6475e5f521b200bb4c72789ea3e64  Sango2SSPseudoCompiler_v0.9.5.zip
077c95832581db5a5af0d98107218437e210d0405ed56a12084f231b07e20a1d  Sango2SSPseudoCompiler_v0.9.6.zip
20987511871310f38bc510f7deffddda2b838a289248927991c16b74b49515e9  Sango2SSPseudoCompiler_v0.9.7.zip
```

## Package contents

The deliveries are not source-only. Depending on version they contain the
compiler and assembler executables, compiler sources, configuration tables,
user documentation, import/template sources, and working examples. A future
release should therefore be assembled from an explicit manifest and tested as
a package; a Git source tag by itself is insufficient to reproduce every old
ZIP byte-for-byte.
