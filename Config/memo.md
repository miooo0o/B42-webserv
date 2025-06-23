
# Fatal vs Skip Criteria for Config Parsing

This document defines criteria for determining whether a parsing error in the configuration file should be treated as **fatal** (i.e., terminate the entire parsing process) or **skippable** (i.e., skip the current block and continue parsing).

---

| Situation                                           | Fatal | Skip | Reason                                                                 |
|-----------------------------------------------------|:-----:|:----:|------------------------------------------------------------------------|
| `server` keyword is missing at the top level        | Yes   | No   | The overall config structure is invalid. Parsing cannot proceed safely. |
| `server` block starts without an opening `{`        | No    | Yes  | Only the current block is malformed; recovery is possible from following lines. |
| `location` block starts without an opening `{`      | No    | Yes  | This is an internal block error; the surrounding `server` block may still be valid. |
| Missing required directives (e.g., `listen`)        | No    | Yes  | Only the current `server` block is invalid; others can still be parsed. |
| Unmatched `{}` braces                               | Yes   | No   | Indicates structural corruption in the config file. Parsing must stop. |
| EOF reached without a closing `}`                   | Yes   | No   | The configuration is incomplete or abruptly terminated. Cannot continue parsing. |
| Recursive `include` directive causes infinite loop  | Yes   | No   | A recursive include loop poses a runtime risk. Must terminate parsing. |
| Unknown directive (e.g., unrecognized keyword)      | No    | Yes  | Likely due to a typo or unsupported directive. Safe to skip the affected block. |

---

**Notes:**

- **Fatal**: Parsing is terminated immediately and no valid configuration is produced.
- **Skip**: The current block is ignored, and parsing continues with the next available block.
