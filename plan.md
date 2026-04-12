1. **Fix 1: Developer Name**
   - Use `sed` to replace `#define DEVELOPER_NAME "Your Name"` with `#define DEVELOPER_NAME "Jinadev R P"` in `srms.c`.
   - Update `print_banner` to use `printf("  |    Developer : %-38s |\n", DEVELOPER_NAME);` and similarly pad `Course` and `Session` lines using `%-38s`.

2. **Fix 2: is_valid_input() NEVER CALLED IN add_student()**
   - The original code has partial implementation for `add_student()`. I will use `replace_with_git_merge_diff` to overwrite the existing block:
   ```c
    /* ── Full validation before committing (Upgrade 3 / Fix 2) ── */
    if (!is_valid_input(ptr, *list, *count, -1)) {
        printf(COLOR_RED
            "  [Error] Student failed validation. "
            "Record NOT added.\n" COLOR_RESET);
        /* Zero out the partially filled slot to keep memory clean */
        memset(ptr, 0, sizeof(Student));
        return;
    }
   ```
   with the exact required text format:
   ```c
    /* Full validation before committing record (Upgrade 3) */
    if (!is_valid_input(ptr, *list, *count, -1)) {
        printf(COLOR_RED
            "  [Error] Student failed validation. "
            "Record NOT added.\n" COLOR_RESET);
        memset(ptr, 0, sizeof(Student));
        return;
    }
   ```
   right before `(*count)++;`.

3. **Fix 3: is_valid_input() NEVER CALLED IN update_student()**
   - In `update_student()`, I will use `replace_with_git_merge_diff` to insert the validation block before `save_to_file`. It will replace the current validation check with the exact requested prompt block:
   ```c
    /* Validate updated record before saving (Upgrade 3) */
    if (!is_valid_input(ptr, list, count, ptr->roll_no)) {
        printf(COLOR_RED
            "  [Error] Updated data failed validation. "
            "Restoring original record.\n" COLOR_RESET);
        *ptr = *temp_copy;
        if (temp_copy != NULL) {
            free(temp_copy);
            temp_copy = NULL;
        }
        return;
    }
    if (temp_copy != NULL) {
        free(temp_copy);
        temp_copy = NULL;
    }
   ```
   replacing the existing `/* ── Validate updated record (Upgrade 3 / Fix 3) ── */` and memory freeing below it, right before `printf(COLOR_GREEN "  [Success]...`.

4. **Fix 4: NO AGE VALIDATION IN add_student()**
   - Use `replace_with_git_merge_diff` to replace the `/* ── Age Input with do-while validation (Fix 4) ── */` block in `add_student()` with the exact 16-35 do-while prompt block specified in the instructions.

5. **Fix 5: NO EMPTY NAME CHECK IN add_student()**
   - In `add_student()`, use `replace_with_git_merge_diff` to ensure the exact text:
   ```c
    if (strlen(ptr->name) < 2) {
        printf(COLOR_RED
            "  [Error] Name must be at least 2 characters. "
            "Student not added.\n" COLOR_RESET);
        return;
    }
   ```
   is present after the `fgets` block for name, replacing the existing partial fix.

6. **Fix 6: NO EMPTY DEPARTMENT CHECK IN add_student()**
   - In `add_student()`, use `replace_with_git_merge_diff` to ensure the exact text:
   ```c
    if (strlen(ptr->department) == 0) {
        printf(COLOR_RED
            "  [Error] Department cannot be empty. "
            "Student not added.\n" COLOR_RESET);
        return;
    }
   ```
   is present after the `fgets` block for department, replacing the existing partial fix.

7. **Fix 7: RECURSIVE MENU STACK OVERFLOW RISK**
   - Use `replace_with_git_merge_diff` to replace the recursive call at the end of `recursive_menu()` with the `operations_counter < 500` guard logic from the prompt. Remove the existing `operations_counter >= 500` guard block at the beginning of the function.

8. **Fix 8: MEMORY LEAK IN update_student()**
   - Review `update_student()` using `cat` and `grep` to identify all return paths after `calloc()` and ensure they include the block:
   ```c
    if (temp_copy != NULL) {
        free(temp_copy);
        temp_copy = NULL;
    }
   ```
   Specifically check lines ~1040 where early exit `return;` is present. Ensure this exact freeing structure replaces existing partial implementation.

9. **Fix 9: NO DELETE CONFIRMATION**
   - Use `replace_with_git_merge_diff` to replace the existing `/* ── Delete confirmation (Fix 9) ── */` block in `delete_student()` with the exact prompt block.

10. **Fix 10: NO WARNING WHEN BINARY SEARCH DESTROYS CGPA SORT**
    - Use `replace_with_git_merge_diff` in `case 5:` of `recursive_menu()` to replace the existing prompt with the exact one specified in the instructions.

11. **Fix 11: POINTER ADDRESS BREAKS BOX ALIGNMENT**
    - Use `replace_with_git_merge_diff` in `demo_storage_classes()` to move the pointer address `%p` printing out of the box border. Replace:
   ```c
        printf("  ║     &a  = address-of operator (see below)                ║\n");
   ```
   with:
   ```c
        printf("  ║     &a  = address-of operator (printed below)    ║\n");
   ```
   and add `printf("  (Unary &) Address of variable a: %p\n", (void *)&a);` after the box closes.

12. **Fix 12: PERFORMANCE BAR LABEL UNCLEAR IN srms_report.cpp**
    - Use `replace_with_git_merge_diff` in `ReportGenerator::printReport()` to replace the specified `cout` lines with the exact prompt output lines.

13. **Fix 13: HARDCODED SUBJECT LABELS ARE FRAGILE**
    - Add `#define SUBJECT_NAMES  "Math,Physics,Chemistry,English,CS"` at top of `srms.c`. Use `replace_with_git_merge_diff` in `display_marks_matrix()` to dynamically print subject headers with `sprintf` and insert the `strtok` demo block specified in the prompt.

14. **Fix 14: fail_count MIXES COUNTS ACROSS STUDENTS**
    - In `is_valid_input()`, use `replace_with_git_merge_diff` to replace `session_fail_count` occurrences and related strings to EXACTLY match the prompt requirements. Note: The current code already uses `session_fail_count`, but the output string is slightly different.

15. **Fix 15: UNION DEMO IS ONLY A COMMENT, NOT LIVE CODE**
    - In `load_sample_data()`, use `replace_with_git_merge_diff` to add the live code implementation directly after the comment block as specified. Remove existing live demonstration code that was manually placed and didn't match the prompt.

16. **Fix 16: FLOATING POINT PRECISION IN CGPA**
    - Check line ~600 (`add_student`) and ~385/400 (`load_sample_data`) where `ptr->cgpa` is assigned, and use `replace_with_git_merge_diff` to replace the existing rounding assignment with the exact one specified:
   `ptr->cgpa = (float)((int)(ptr->cgpa * 100.0f + 0.5f)) / 100.0f;`

17. **Fix 17: PERFORMANCE % FORMULA HAS NO EXPLANATION**
    - Use `replace_with_git_merge_diff` to add the exact block comment above `int filled = (int)(cgpa * 2);` in `srms_report.cpp`.

18. **Fix 18: BANNER BOX MISALIGNS WITH DIFFERENT NAME LENGTHS**
    - Covered by Fix 1 (formatting `print_banner`). Verify all fixed lines have `% -Ns` pad.

19. **Compile and Verify**
    - Run `gcc -Wall -Wextra -o srms srms.c` and `g++ -Wall -Wextra -o srms_report srms_report.cpp` via `run_in_bash_session`.

20. **Setup Tests**
    - Run bash script to create `test_inputs.txt` exactly as provided.
    - Run `cat test_inputs.txt` to verify contents.

21. **Run C Automation Tests**
    - Run `./srms < test_inputs.txt > test_output.txt 2>&1` via `run_in_bash_session`.
    - Verify with bash commands: `cat test_output.txt | grep FATAL`, `grep Segmentation`, and check output visually with `head -n 50 test_output.txt`.

22. **Run C++ Automation Tests**
    - Run `echo -e "Jinadev R P\n9.2\nArjun Sharma\n7.5" | ./srms_report > cpp_test_output.txt 2>&1` via bash session.
    - Verify output using `cat cpp_test_output.txt`.

23. **Pre Commit**
    - Complete pre-commit steps to ensure proper testing, verification, review, and reflection are done.

24. **Submit Change**
    - Use `submit` tool to push branch with `"fix: apply all 18 corrections + verified test run — Jinadev R P"` as title and description.
