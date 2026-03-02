## 2025-05-14 - [CRLF and Encoding Pitfalls]
**Learning:** Automated file modification tools like `replace_with_git_merge_diff` can fail or cause total file overwrites when dealing with CRLF line endings if not handled with exact precision. Furthermore, modifying localized files (e.g., `uncap_ko.c`) requires strict UTF-8 preservation to avoid corrupting non-ASCII comments into binary-escaped gibberish.
**Action:** Use custom Python scripts for complex search-and-replace tasks in Windows-style codebases, ensuring they explicitly handle line endings and UTF-8 encoding.

## 2025-05-14 - [High-Frequency Path Optimizations]
**Learning:** In low-level system utilities like key mappers, every instruction in the hook path matters. Replacing `strcpy` with pointer assignments for static literals and replacing `fflush(NULL)` with targeted `fflush(stream)` inside conditionals significantly reduces latency and I/O pressure on every keystroke.
**Action:** Always audit I/O flushing and string operations in hot paths (loops or event hooks) for more efficient alternatives.
