/******************************************************************************
 *  ╔══════════════════════════════════════════════════════════════════════╗
 *  ║         SRMS — Student Record Management System                    ║
 *  ║         CSE101 — C Programming Academic Project                    ║
 *  ║         Single-File Console Application in ANSI C                  ║
 *  ╚══════════════════════════════════════════════════════════════════════╝
 *
 *  Author  : CSE101 Student
 *  Date    : April 2026
 *  Compiler: gcc -Wall -o srms srms.c
 *
 *  SYLLABUS COVERAGE:
 *    Unit I   — Data Types, Operators, Structures, Unions
 *    Unit II  — Control Structures, I/O
 *    Unit III — Functions, Storage Classes
 *    Unit IV  — Arrays, Searching, Sorting
 *    Unit V   — Pointers, Strings, Dynamic Memory
 *    Unit VI  — Structures & Unions (advanced), C++ basics
 ******************************************************************************/

/* ═══════════════════════════════════════════════════════════════════════════
 *  SECTION 1: INCLUDES
 * ═══════════════════════════════════════════════════════════════════════════ */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

/* ═══════════════════════════════════════════════════════════════════════════
 *  SECTION 2: PREPROCESSOR CONSTANTS (#define — Unit I)
 * ═══════════════════════════════════════════════════════════════════════════ */
#define MAX_NAME         50
#define MAX_DEPT         30
#define MAX_CITY         30
#define MAX_STATE        30
#define NUM_SUBJECTS     5
#define INITIAL_CAPACITY 5
#define LINE_SEP         "  ─────────────────────────────────────────────────────────────────────────────────────────────"
#define FILE_NAME        "srms_data.dat"
#define DEVELOPER_NAME   "Jinadev R P"
#define SUBJECT_NAMES    "Math,Physics,Chemistry,English,CS"

/* ── ANSI Terminal Color Codes (Upgrade 2) ── */
#define COLOR_RED        "\x1b[31m"
#define COLOR_GREEN      "\x1b[32m"
#define COLOR_YELLOW     "\x1b[33m"
#define COLOR_BLUE       "\x1b[34m"
#define COLOR_MAGENTA    "\x1b[35m"
#define COLOR_CYAN       "\x1b[36m"
#define COLOR_BOLD       "\x1b[1m"
#define COLOR_RESET      "\x1b[0m"

/* ═══════════════════════════════════════════════════════════════════════════
 *  SECTION 3: UNION DEFINITION (Unit VI — Demonstrates Union Usage)
 *
 *  A union stores EITHER grade (char) OR pass_status (int), NOT both.
 *  Both members share the same memory location. Only the last-written
 *  member holds a valid value at any given time.
 * ═══════════════════════════════════════════════════════════════════════════ */
typedef union result_t {
    char grade;         /* e.g., 'A', 'B', 'O', 'F' */
    int  pass_status;   /* 1 = pass, 0 = fail        */
} result_t;

/* ═══════════════════════════════════════════════════════════════════════════
 *  SECTION 4: STRUCT DEFINITIONS (Unit I + Unit VI — Nested Structs)
 * ═══════════════════════════════════════════════════════════════════════════ */

/* Nested struct: Address */
typedef struct Address {
    char city[MAX_CITY];
    char state[MAX_STATE];
} Address;

/* Nested struct: Marks — stores 5 subject marks in a float array (Unit IV) */
typedef struct Marks {
    float subjects[NUM_SUBJECTS];   /* subjects[0..4] */
} Marks;

/* Main struct: Student — uses typedef (Unit I) */
typedef struct Student {
    int       roll_no;              /* int data type      */
    char      name[MAX_NAME];       /* char array (string)*/
    int       age;                  /* int data type      */
    float     cgpa;                 /* float data type    */
    char      department[MAX_DEPT]; /* char array (string)*/
    Address   address;              /* nested struct      */
    Marks     marks;                /* nested struct      */
    result_t  result;               /* union member       */
} Student;

/* ═══════════════════════════════════════════════════════════════════════════
 *  SECTION 5: GLOBAL VARIABLES (Unit III — extern storage class)
 *
 *  'total_students' is defined here at file scope.
 *  Inside functions, we reference it with 'extern' to demonstrate
 *  the extern storage class in a single-file context.
 * ═══════════════════════════════════════════════════════════════════════════ */
int total_students  = 0;    /* extern demonstration: global student count */
int current_capacity = 0;   /* current allocated capacity                */

/* ═══════════════════════════════════════════════════════════════════════════
 *  SECTION 6: FUNCTION PROTOTYPES (Unit III — declared at the top)
 *  Every function used in the program is prototyped here.
 * ═══════════════════════════════════════════════════════════════════════════ */
void   print_banner(void);
void   flush_input(void);
void   string_to_upper(char *str);
char   calculate_grade(float cgpa);
void   load_sample_data(Student **list, int *count, int *capacity);
void   add_student(Student **list, int *count, int *capacity);
void   display_all_students(Student *list, int count);
void   display_marks_matrix(Student *list, int count);
int    search_linear(Student *list, int count, const char *name);
void   sort_by_roll(Student *list, int count);
int    search_binary(Student *list, int count, int roll_no);
void   sort_bubble(Student *list, int count);
void   update_student(Student *list, int count);
void   delete_student(Student **list, int *count);
void   display_statistics(Student *list, int count);
void   demo_storage_classes(void);
void   swap_by_value(int a, int b);
void   swap_by_address(int *a, int *b);
void   demo_call_types(void);
void   demo_pointer_arithmetic(Student *list, int count);
void   recursive_menu(Student **list, int *count, int *capacity);
/* ── Upgrade function prototypes ── */
void   save_to_file(Student *arr, int count);
void   load_from_file(Student **arr, int *count, int *capacity);
int    is_valid_input(Student *s, Student *list, int count, int self_roll);
void   export_report(Student *arr, int count);

/* ═══════════════════════════════════════════════════════════════════════════
 *  SECTION 7: MAIN FUNCTION
 *
 *  Purpose : Program entry point. Allocates initial memory with malloc(),
 *            loads sample data, starts recursive menu, and frees memory.
 *  Params  : None
 *  Returns : 0 on success, 1 on allocation failure
 *  Covers  : Unit V (malloc, free), Unit III (main structure)
 * ═══════════════════════════════════════════════════════════════════════════ */
int main(void)
{
    Student *students = NULL;

    /* ── malloc() — initial allocation for INITIAL_CAPACITY students (Unit V) ── */
    students = (Student *)malloc(INITIAL_CAPACITY * sizeof(Student));
    if (students == NULL) {
        fprintf(stderr, COLOR_RED "  [FATAL] Initial memory allocation failed!\n" COLOR_RESET);
        return 1;
    }
    current_capacity = INITIAL_CAPACITY;

    printf("\n");
    print_banner();
    puts("  Initializing system...");                   /* puts() usage #1 (Unit II) */

    /* ── Upgrade 1: Try loading from file first ── */
    load_from_file(&students, &total_students, &current_capacity);
    if (total_students == 0) {
        /* No saved data — load sample data for demo */
        load_sample_data(&students, &total_students, &current_capacity);
        printf(COLOR_CYAN "  No saved data found. Loaded %d sample students.\n" COLOR_RESET, total_students);
    } else {
        printf(COLOR_GREEN "  Loaded %d students from %s.\n" COLOR_RESET, total_students, FILE_NAME);
    }
    printf("  System ready.\n\n");

    /* Start the recursive menu — calls itself until user selects Exit */
    recursive_menu(&students, &total_students, &current_capacity);

    /* ── Auto-save before exit ── */
    save_to_file(students, total_students);

    /* ── free() — clean up all dynamic memory on exit (Unit V) ── */
    free(students);
    students = NULL;    /* Prevent dangling pointer */

    /*
     * ═══════════════════════════════════════════════════════════════════
     *  DANGLING POINTER DEMONSTRATION (Unit V):
     * ═══════════════════════════════════════════════════════════════════
     *  After free(students), the pointer 'students' would still hold
     *  the old memory address if we hadn't set it to NULL.
     *  Accessing memory through a dangling pointer causes UNDEFINED
     *  BEHAVIOR — the program may crash, corrupt data, or appear to
     *  work but produce wrong results.
     *
     *  Example of the bug (DO NOT execute):
     *
     *      free(students);
     *      // students is now DANGLING — points to freed memory
     *      printf("%d", students->roll_no);   // UNDEFINED BEHAVIOR!
     *      students->age = 25;                 // UNDEFINED BEHAVIOR!
     *
     *  Best practice: ALWAYS set pointer to NULL after free().
     *      free(students);
     *      students = NULL;   // Now safe — dereferencing NULL crashes
     *                         // predictably (segfault) instead of
     *                         // silently corrupting memory.
     * ═══════════════════════════════════════════════════════════════════
     */

    /* Colored cleanup message (Upgrade 2) */
    printf(COLOR_CYAN "\n  [Memory] All dynamic memory freed successfully.\n" COLOR_RESET);

    puts("\n  Thank you for using SRMS! Goodbye.\n");   /* puts() usage #2 (Unit II) */
    return 0;
}

/* ═══════════════════════════════════════════════════════════════════════════
 *  FUNCTION: print_banner
 *
 *  Purpose : Displays the startup ASCII art banner
 *  Params  : None
 *  Returns : void
 *  Covers  : Unit II (formatted printf output)
 * ═══════════════════════════════════════════════════════════════════════════ */
void print_banner(void)
{
    printf(COLOR_CYAN COLOR_BOLD);
    printf("  +========================================================+\n");
    printf("  |                                                        |\n");
    printf("  |    ███████╗██████╗ ███╗   ███╗███████╗                 |\n");
    printf("  |    ██╔════╝██╔══██╗████╗ ████║██╔════╝                 |\n");
    printf("  |    ███████╗██████╔╝██╔████╔██║███████╗                 |\n");
    printf("  |    ╚════██║██╔══██╗██║╚██╔╝██║╚════██║                 |\n");
    printf("  |    ███████║██║  ██║██║ ╚═╝ ██║███████║                 |\n");
    printf("  |    ╚══════╝╚═╝  ╚═╝╚═╝     ╚═╝╚══════╝                 |\n");
    printf("  |                                                        |\n");
    printf("  |    Student Record Management System                    |\n");
    printf("  |    Developer : %-38s |\n", DEVELOPER_NAME);
    printf("  |    Course    : %-38s |\n", "CSE101 - Computer Programming");
    printf("  |    Session   : %-38s |\n", "2025-26");
    printf("  |                                                        |\n");
    printf("  +========================================================+\n");
    printf(COLOR_RESET);
}

/* ═══════════════════════════════════════════════════════════════════════════
 *  FUNCTION: flush_input
 *
 *  Purpose : Clears the stdin buffer after scanf() to prevent leftover
 *            characters from being read by subsequent fgets() calls.
 *  Params  : None
 *  Returns : void
 *  Covers  : Unit II (getchar() for buffer flush)
 * ═══════════════════════════════════════════════════════════════════════════ */
void flush_input(void)
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)    /* getchar() usage (Unit II) */
        ;   /* discard characters until newline or EOF */
}

/* ═══════════════════════════════════════════════════════════════════════════
 *  FUNCTION: string_to_upper
 *
 *  Purpose : Converts a string to uppercase manually using character
 *            arithmetic — WITHOUT using toupper() from <ctype.h>.
 *  Params  : str — pointer to the string to convert (modified in place)
 *  Returns : void
 *  Covers  : Unit V (strings, character arithmetic, pointer traversal)
 * ═══════════════════════════════════════════════════════════════════════════ */
void string_to_upper(char *str)
{
    /* Traverse using pointer — while loop (Unit II) */
    while (*str) {
        if (*str >= 'a' && *str <= 'z') {           /* relational operators (Unit I) */
            *str = *str - 'a' + 'A';               /* character arithmetic  */
            /* 'a' = 97, 'A' = 65;  diff = 32 */
        }
        str++;  /* pointer arithmetic — move to next character; unary ++ (Unit I) */
    }
}

/* ═══════════════════════════════════════════════════════════════════════════
 *  FUNCTION: calculate_grade
 *
 *  Purpose : Returns a grade character based on CGPA using the grading
 *            scale: O(>=9), A(>=8), B(>=7), C(>=6), D(>=5), F(<5).
 *  Params  : cgpa — the computed CGPA (float)
 *  Returns : char — the grade character ('O','A','B','C','D', or 'F')
 *  Covers  : Unit I (relational operators), Unit III (function with return)
 * ═══════════════════════════════════════════════════════════════════════════ */
char calculate_grade(float cgpa)
{
    if (cgpa >= 9.0f) return 'O';   /* Outstanding */
    if (cgpa >= 8.0f) return 'A';
    if (cgpa >= 7.0f) return 'B';
    if (cgpa >= 6.0f) return 'C';
    if (cgpa >= 5.0f) return 'D';   /* Pass */
    return 'F';                     /* Fail */
}

/* ═══════════════════════════════════════════════════════════════════════════
 *  FUNCTION: load_sample_data
 *
 *  Purpose : Pre-loads 3 sample students into the array for demo purposes.
 *            Uses pointer notation (->) for all struct access (Unit V).
 *  Params  : list     — pointer to the student array pointer
 *            count    — pointer to current student count
 *            capacity — pointer to current array capacity
 *  Returns : void
 *  Covers  : Unit V (pointers, strcpy), Unit IV (array initialization),
 *            Unit VI (nested struct & union access)
 * ═══════════════════════════════════════════════════════════════════════════ */
void load_sample_data(Student **list, int *count, int *capacity)
{
    Student *ptr;
    float sum;
    int i;

    /* ── Student 1: Rahul Sharma — Grade A ── */
    ptr = *list + 0;                         /* pointer arithmetic (Unit V) */
    ptr->roll_no = 101;
    strcpy(ptr->name, "Rahul Sharma");       /* strcpy() usage (Unit V)     */
    ptr->age = 20;
    strcpy(ptr->department, "CSE");
    strcpy(ptr->address.city, "Mumbai");
    strcpy(ptr->address.state, "Maharashtra");
    ptr->marks.subjects[0] = 85.0f;
    ptr->marks.subjects[1] = 90.0f;
    ptr->marks.subjects[2] = 78.0f;
    ptr->marks.subjects[3] = 92.0f;
    ptr->marks.subjects[4] = 88.0f;
    sum = 0.0f;
    for (i = 0; i < NUM_SUBJECTS; i++) sum += ptr->marks.subjects[i]; /* arithmetic + (Unit I) */
    ptr->cgpa = (sum / NUM_SUBJECTS) / 10.0f;   /* arithmetic / (Unit I) */
    ptr->cgpa = (float)((int)(ptr->cgpa * 100 + 0.5f)) / 100.0f; /* Fix 16: round to 2dp */
    ptr->result.grade = calculate_grade(ptr->cgpa);

    /* ── Student 2: Priya Patel — Grade O ── */
    ptr = *list + 1;
    ptr->roll_no = 102;
    strcpy(ptr->name, "Priya Patel");
    ptr->age = 21;
    strcpy(ptr->department, "ECE");
    strcpy(ptr->address.city, "Delhi");
    strcpy(ptr->address.state, "Delhi");
    ptr->marks.subjects[0] = 95.0f;
    ptr->marks.subjects[1] = 92.0f;
    ptr->marks.subjects[2] = 98.0f;
    ptr->marks.subjects[3] = 90.0f;
    ptr->marks.subjects[4] = 94.0f;
    sum = 0.0f;
    for (i = 0; i < NUM_SUBJECTS; i++) sum += ptr->marks.subjects[i];
    ptr->cgpa = (sum / NUM_SUBJECTS) / 10.0f;
    ptr->cgpa = (float)((int)(ptr->cgpa * 100 + 0.5f)) / 100.0f;
    ptr->result.grade = calculate_grade(ptr->cgpa);

    /* ── Student 3: Amit Kumar — Grade B ── */
    ptr = *list + 2;
    ptr->roll_no = 103;
    strcpy(ptr->name, "Amit Kumar");
    ptr->age = 19;
    strcpy(ptr->department, "ME");
    strcpy(ptr->address.city, "Pune");
    strcpy(ptr->address.state, "Maharashtra");
    ptr->marks.subjects[0] = 72.0f;
    ptr->marks.subjects[1] = 78.0f;
    ptr->marks.subjects[2] = 70.0f;
    ptr->marks.subjects[3] = 68.0f;
    ptr->marks.subjects[4] = 75.0f;
    sum = 0.0f;
    for (i = 0; i < NUM_SUBJECTS; i++) sum += ptr->marks.subjects[i];
    ptr->cgpa = (sum / NUM_SUBJECTS) / 10.0f;
    ptr->cgpa = (float)((int)(ptr->cgpa * 100 + 0.5f)) / 100.0f;
    ptr->result.grade = calculate_grade(ptr->cgpa);

    /* ── Student 4: Sneha Gupta — Grade A ── */
    ptr = *list + 3;
    ptr->roll_no = 104;
    strcpy(ptr->name, "Sneha Gupta");
    ptr->age = 20;
    strcpy(ptr->department, "IT");
    strcpy(ptr->address.city, "Bangalore");
    strcpy(ptr->address.state, "Karnataka");
    ptr->marks.subjects[0] = 88.0f;
    ptr->marks.subjects[1] = 82.0f;
    ptr->marks.subjects[2] = 85.0f;
    ptr->marks.subjects[3] = 80.0f;
    ptr->marks.subjects[4] = 90.0f;
    sum = 0.0f;
    for (i = 0; i < NUM_SUBJECTS; i++) sum += ptr->marks.subjects[i];
    ptr->cgpa = (sum / NUM_SUBJECTS) / 10.0f;
    ptr->cgpa = (float)((int)(ptr->cgpa * 100 + 0.5f)) / 100.0f;
    ptr->result.grade = calculate_grade(ptr->cgpa);

    /* ── Student 5: Vikram Singh — Grade F ── */
    ptr = *list + 4;
    ptr->roll_no = 105;
    strcpy(ptr->name, "Vikram Singh");
    ptr->age = 22;
    strcpy(ptr->department, "CIVIL");
    strcpy(ptr->address.city, "Jaipur");
    strcpy(ptr->address.state, "Rajasthan");
    ptr->marks.subjects[0] = 30.0f;
    ptr->marks.subjects[1] = 45.0f;
    ptr->marks.subjects[2] = 38.0f;
    ptr->marks.subjects[3] = 25.0f;
    ptr->marks.subjects[4] = 42.0f;
    sum = 0.0f;
    for (i = 0; i < NUM_SUBJECTS; i++) sum += ptr->marks.subjects[i];
    ptr->cgpa = (sum / NUM_SUBJECTS) / 10.0f;
    ptr->cgpa = (float)((int)(ptr->cgpa * 100 + 0.5f)) / 100.0f;
    ptr->result.grade = calculate_grade(ptr->cgpa);

    *count = 5;

    /* Union Demonstration (Unit VI): ... */
    /* We can alternatively set pass_status: ... */

    /* ── Live Union Demo: show shared memory overwrite ── */
    printf(COLOR_CYAN
        "\n  [Union Live Demo — Unit VI]\n" COLOR_RESET);
    printf("  Vikram's result.grade  = '%c' (set by "
           "calculate_grade)\n",
           (*list + 4)->result.grade);
    (*list + 4)->result.pass_status = 0;
    printf("  After: result.pass_status = %d  "
           "(overwrites grade — same memory)\n",
           (*list + 4)->result.pass_status);
    printf("  result.grade now = '%c'  "
           "(invalid — union memory overwritten)\n",
           (*list + 4)->result.grade);
    printf("  Restoring grade...\n");
    (*list + 4)->result.grade =
        calculate_grade((*list + 4)->cgpa);
    printf("  result.grade restored = '%c'\n\n",
           (*list + 4)->result.grade);
}

/* ═══════════════════════════════════════════════════════════════════════════
 *  FUNCTION: add_student
 *
 *  Purpose : Adds a new student to the dynamic array. Demonstrates:
 *            - realloc() for capacity expansion (Unit V)
 *            - fgets() for string input with newline removal (Unit V)
 *            - strlen(), strcpy(), strcat(), strtok() (Unit V)
 *            - goto for input error retry (Unit II)
 *            - do-while for marks validation (Unit II)
 *            - Pointer notation (->) for struct access (Unit V)
 *  Params  : list     — pointer to student array pointer (may be realloced)
 *            count    — pointer to current student count
 *            capacity — pointer to allocated capacity
 *  Returns : void
 *  Covers  : Unit II, Unit IV (array insert), Unit V
 * ═══════════════════════════════════════════════════════════════════════════ */
void add_student(Student **list, int *count, int *capacity)
{
    Student *ptr;
    float sum = 0.0f;
    float mark;
    int i, roll;
    char name_copy[MAX_NAME];       /* for strtok demonstration */
    char greeting[100] = "Hello, "; /* for strcat demonstration */
    char *first_name;

    printf("\n%s\n", LINE_SEP);
    printf("  ADD NEW STUDENT\n");
    printf("%s\n\n", LINE_SEP);

    /* ── Check capacity — realloc() if full (Unit V) ── */
    if (*count >= *capacity) {
        *capacity *= 2;     /* double the capacity */
        *list = (Student *)realloc(*list, (*capacity) * sizeof(Student));
        if (*list == NULL) {
            fprintf(stderr, "  [FATAL] Memory reallocation failed!\n");
            exit(1);
        }
        printf("  [Memory] Capacity expanded to %d students (realloc).\n", *capacity);
    }

    ptr = *list + *count;   /* pointer arithmetic: point to next free slot (Unit V) */

    /* ── Roll Number Input with goto for error retry (Unit II) ── */
input_error:                /* goto label — used for input validation retry (Unit II) */
    printf("  Enter Roll Number (positive integer): ");
    if (scanf("%d", &roll) != 1 || roll <= 0) {     /* relational <= , logical || (Unit I) */
        printf(COLOR_RED "  [Error] Invalid roll number! Must be a positive integer.\n" COLOR_RESET);
        flush_input();
        goto input_error;   /* goto — retry input on error (Unit II — used ONCE only) */
    }
    flush_input();

    /* Check for duplicate roll number */
    for (i = 0; i < *count; i++) {
        if ((*list + i)->roll_no == roll) {         /* relational == (Unit I) */
            printf(COLOR_RED "  [Error] Roll number %d already exists! Try again.\n" COLOR_RESET, roll);
            goto input_error;
        }
    }
    ptr->roll_no = roll;        /* assignment operator = (Unit I) */

    /* ── Name Input using fgets() + newline removal with strlen() (Unit V) ── */
    printf("  Enter Name: ");
    fgets(ptr->name, MAX_NAME, stdin);              /* fgets() for string input (Unit V) */
    {
        size_t len = strlen(ptr->name);             /* strlen() usage (Unit V) */
        if (len > 0 && ptr->name[len - 1] == '\n') /* logical && (Unit I)     */
            ptr->name[len - 1] = '\0';             /* manually clean newline  */
    }

    if (strlen(ptr->name) < 2) {
        printf(COLOR_RED
            "  [Error] Name must be at least 2 characters. "
            "Student not added.\n" COLOR_RESET);
        return;
    }

    /* ── strtok() demonstration: extract first name (Unit V) ── */
    strcpy(name_copy, ptr->name);                   /* strcpy() usage (Unit V)  */
    first_name = strtok(name_copy, " ");            /* strtok() usage (Unit V)  */
    if (first_name != NULL) {
        strcat(greeting, first_name);               /* strcat() usage (Unit V)  */
        strcat(greeting, "! Welcome to SRMS.");
        printf("  %s\n", greeting);
    }

    do {
        printf("  Enter Age (16-35): ");
        if (scanf("%d", &ptr->age) != 1) {
            printf(COLOR_RED
                "  [Error] Invalid input. Enter a number.\n"
                COLOR_RESET);
            flush_input();
            ptr->age = 0;
            continue;
        }
        flush_input();
        if (ptr->age < 16 || ptr->age > 35)
            printf(COLOR_RED
                "  [Error] Age must be between 16 and 35.\n"
                COLOR_RESET);
    } while (ptr->age < 16 || ptr->age > 35);

    /* ── Department Input using fgets() ── */
    printf("  Enter Department: ");
    fgets(ptr->department, MAX_DEPT, stdin);
    {
        size_t len = strlen(ptr->department);
        if (len > 0 && ptr->department[len - 1] == '\n')
            ptr->department[len - 1] = '\0';
    }

    if (strlen(ptr->department) == 0) {
        printf(COLOR_RED
            "  [Error] Department cannot be empty. "
            "Student not added.\n" COLOR_RESET);
        return;
    }

    /* ── Address Input ── */
    printf("  Enter City: ");
    fgets(ptr->address.city, MAX_CITY, stdin);
    {
        size_t len = strlen(ptr->address.city);
        if (len > 0 && ptr->address.city[len - 1] == '\n')
            ptr->address.city[len - 1] = '\0';
    }
    printf("  Enter State: ");
    fgets(ptr->address.state, MAX_STATE, stdin);
    {
        size_t len = strlen(ptr->address.state);
        if (len > 0 && ptr->address.state[len - 1] == '\n')
            ptr->address.state[len - 1] = '\0';
    }

    /* ── Subject Marks Input with do-while validation (Unit II) ── */
    printf("  Enter marks for %d subjects (0-100):\n", NUM_SUBJECTS);
    for (i = 0; i < NUM_SUBJECTS; i++) {
        do {                                        /* do-while loop (Unit II) */
            printf("    Subject %d: ", i + 1);      /* arithmetic + (Unit I)   */
            scanf("%f", &mark);
            flush_input();
            if (mark < 0.0f || mark > 100.0f)      /* logical || (Unit I) */
                printf(COLOR_RED "    [Error] Marks must be between 0 and 100.\n" COLOR_RESET);
        } while (mark < 0.0f || mark > 100.0f);    /* do-while (Unit II) */

        ptr->marks.subjects[i] = mark;
        sum += mark;                                /* assignment += (Unit I) */
    }

    /* ── Compute CGPA and Grade ── */
    ptr->cgpa = (sum / NUM_SUBJECTS) / 10.0f;      /* arithmetic / (Unit I) */
    ptr->cgpa = (float)((int)(ptr->cgpa * 100 + 0.5f)) / 100.0f; /* Fix 16: round */
    ptr->result.grade = calculate_grade(ptr->cgpa);

    /* Union demonstration: alternatively set pass_status */
    /* ptr->result.pass_status = (ptr->cgpa >= 5.0f) ? 1 : 0;  */
    /* ^ This would OVERWRITE grade since they share memory in the union */

    /* Full validation before committing record (Upgrade 3) */
    if (!is_valid_input(ptr, *list, *count, -1)) {
        printf(COLOR_RED
            "  [Error] Student failed validation. "
            "Record NOT added.\n" COLOR_RESET);
        memset(ptr, 0, sizeof(Student));
        return;
    }

    (*count)++;     /* unary ++ (Unit I) */

    printf(COLOR_GREEN "\n  [Success] Student '%s' (Roll #%d) added successfully!\n" COLOR_RESET, ptr->name, ptr->roll_no);
    printf("  CGPA: %.2f | Grade: %c | Status: %s\n",
           ptr->cgpa, ptr->result.grade,
           ptr->cgpa >= 5.0f ? "PASS" : "FAIL");   /* ternary operator (Unit I) */

    /* Auto-save to file (Upgrade 1) */
    save_to_file(*list, *count);
}

/* ═══════════════════════════════════════════════════════════════════════════
 *  FUNCTION: display_all_students
 *
 *  Purpose : Displays all students in a formatted, column-aligned table.
 *            Uses while loop with explicit index counter (Unit II).
 *            Demonstrates strcat() for building location string.
 *  Params  : list  — pointer to student array
 *            count — number of students
 *  Returns : void
 *  Covers  : Unit II (formatted printf, while loop, continue),
 *            Unit V (pointer notation, strcat, strcmp)
 * ═══════════════════════════════════════════════════════════════════════════ */
void display_all_students(Student *list, int count)
{
    int idx = 0;
    Student *ptr;
    char full_location[70];

    printf("\n%s\n", LINE_SEP);
    printf("  ALL STUDENTS — TABLE VIEW\n");
    printf("%s\n\n", LINE_SEP);

    if (count == 0) {
        printf("  No students in the system.\n");
        return;
    }

    /* Column-aligned header (Unit II — formatted printf) */
    printf(COLOR_BLUE COLOR_BOLD);
    printf("  %-6s %-20s %-5s %-7s %-12s %-6s %-8s %-25s\n",
           "Roll", "Name", "Age", "CGPA", "Department", "Grade", "Status", "Location");
    printf("  %-6s %-20s %-5s %-7s %-12s %-6s %-8s %-25s\n",
           "------", "--------------------", "-----", "-------",
           "------------", "------", "--------", "-------------------------");
    printf(COLOR_RESET);

    /* while loop with explicit index (Unit II) */
    while (idx < count) {
        ptr = list + idx;   /* pointer arithmetic (Unit V) */

        /* continue demonstration: skip students with empty names (Unit II) */
        if (strlen(ptr->name) == 0) {
            idx++;
            continue;       /* continue — skip this iteration (Unit II) */
        }

        /* Build full location using strcat (Unit V) */
        strcpy(full_location, ptr->address.city);
        strcat(full_location, ", ");                /* strcat() (Unit V) */
        strcat(full_location, ptr->address.state);

        /* Color based on grade: O/A=GREEN, F=RED, D=YELLOW (Upgrade 2) */
        if (ptr->result.grade == 'O' || ptr->result.grade == 'A')
            printf(COLOR_GREEN);
        else if (ptr->result.grade == 'F')
            printf(COLOR_RED);
        else if (ptr->result.grade == 'D')
            printf(COLOR_YELLOW);

        /* Formatted row output */
        printf("  %-6d %-20s %-5d %-7.2f %-12s   %-4c %-8s %-25s\n",
               ptr->roll_no,
               ptr->name,
               ptr->age,
               ptr->cgpa,
               ptr->department,
               ptr->result.grade,
               (ptr->cgpa >= 5.0f) ? "PASS" : "FAIL",  /* ternary (Unit I) */
               full_location);
        printf(COLOR_RESET);

        idx++;  /* unary ++ (Unit I) */
    }

    printf("\n  Total students: %d\n", count);
}

/* ═══════════════════════════════════════════════════════════════════════════
 *  FUNCTION: display_marks_matrix
 *
 *  Purpose : Displays a 2D matrix of students × subjects (Unit IV).
 *            Rows = students, Columns = 5 subjects.
 *  Params  : list  — pointer to student array
 *            count — number of students
 *  Returns : void
 *  Covers  : Unit IV (2D array display), Unit II (for loop)
 * ═══════════════════════════════════════════════════════════════════════════ */
void display_marks_matrix(Student *list, int count)
{
    int i, j;
    Student *ptr;
    float row_sum;

    printf("\n%s\n", LINE_SEP);
    printf("  MARKS MATRIX \u2014 2D ARRAY VIEW (Students x Subjects)\n");
    printf("%s\n\n", LINE_SEP);

    if (count == 0) {
        printf("  No students in the system.\n");
        return;
    }

    /* Generate column headers dynamically from NUM_SUBJECTS */
    printf("  %-20s", "Name");
    for (j = 0; j < NUM_SUBJECTS; j++) {
        char sub_label[15];
        sprintf(sub_label, "Subject-%d", j + 1);
        printf("  %-12s", sub_label);
    }
    printf("  %-8s\n", "Average");
    printf("  %-20s", "--------------------");
    for (j = 0; j < NUM_SUBJECTS; j++)
        printf("  %-12s", "------------");
    printf("  %-8s\n", "--------");

    /* \u2500\u2500 Print each student's marks row (for loop \u2014 Unit II) \u2500\u2500 */
    for (i = 0; i < count; i++) {
        ptr = list + i;     /* pointer arithmetic (Unit V) */
        printf("  %-20s", ptr->name);

        row_sum = 0.0f;
        for (j = 0; j < NUM_SUBJECTS; j++) {
            printf("  %-12.1f", ptr->marks.subjects[j]);
            row_sum += ptr->marks.subjects[j];
        }
        printf("  %-8.1f\n", row_sum / NUM_SUBJECTS);  /* arithmetic / (Unit I) */
    }

    printf("\n  Matrix dimensions: %d students x %d subjects\n", count, NUM_SUBJECTS);

    /* strtok() demonstration (Unit V) */
    printf("\n  Subject Name Reference (strtok demo — Unit V):\n");
    {
        char labels_copy[100];
        char *tok;
        int col_num = 1;
        strcpy(labels_copy, SUBJECT_NAMES);
        tok = strtok(labels_copy, ",");
        while (tok != NULL) {
            printf("  Subject-%d = %s\n", col_num++, tok);
            tok = strtok(NULL, ",");
        }
    }
}

/* ═══════════════════════════════════════════════════════════════════════════
 *  FUNCTION: search_linear
 *
 *  Purpose : Searches for a student by name using linear search (Unit IV).
 *            Uses strcmp() for string comparison (Unit V).
 *  Params  : list  — pointer to student array
 *            count — number of students
 *            name  — the name to search for (const char *)
 *  Returns : int   — index of found student, or -1 if not found
 *  Covers  : Unit IV (linear search), Unit V (strcmp), Unit II (break)
 * ═══════════════════════════════════════════════════════════════════════════ */
int search_linear(Student *list, int count, const char *name)
{
    int i;
    int found = -1;         /* -1 indicates not found */
    char upper_name[MAX_NAME];
    char upper_student[MAX_NAME];

    /* Make a copy and convert to upper for case-insensitive comparison */
    strcpy(upper_name, name);
    string_to_upper(upper_name);

    for (i = 0; i < count; i++) {
        strcpy(upper_student, (list + i)->name);    /* pointer notation (Unit V) */
        string_to_upper(upper_student);

        if (strcmp(upper_student, upper_name) == 0) { /* strcmp() usage (Unit V) */
            found = i;
            break;  /* break — exit loop once found (Unit II) */
        }
    }

    return found;
}

/* ═══════════════════════════════════════════════════════════════════════════
 *  FUNCTION: sort_by_roll
 *
 *  Purpose : Sorts the student array by roll_no (ascending) using
 *            selection sort. Required before binary search (Unit IV).
 *  Params  : list  — pointer to student array
 *            count — number of students
 *  Returns : void
 *  Covers  : Unit IV (sorting), Unit V (pointer access)
 * ═══════════════════════════════════════════════════════════════════════════ */
void sort_by_roll(Student *list, int count)
{
    int i, j, min_idx;
    Student temp;

    for (i = 0; i < count - 1; i++) {               /* arithmetic - (Unit I) */
        min_idx = i;
        for (j = i + 1; j < count; j++) {
            if ((list + j)->roll_no < (list + min_idx)->roll_no)  /* relational < (Unit I) */
                min_idx = j;
        }
        if (min_idx != i) {                          /* relational != (Unit I) */
            temp = *(list + i);
            *(list + i) = *(list + min_idx);
            *(list + min_idx) = temp;
        }
    }
}

/* ═══════════════════════════════════════════════════════════════════════════
 *  FUNCTION: search_binary
 *
 *  Purpose : Searches for a student by roll number using binary search.
 *            PRECONDITION: Array must be sorted by roll_no first.
 *  Params  : list    — pointer to student array (sorted by roll_no)
 *            count   — number of students
 *            roll_no — roll number to search for
 *  Returns : int     — index of found student, or -1 if not found
 *  Covers  : Unit IV (binary search), Unit I (arithmetic operators)
 * ═══════════════════════════════════════════════════════════════════════════ */
int search_binary(Student *list, int count, int roll_no)
{
    int low = 0;
    int high = count - 1;
    int mid;

    while (low <= high) {                           /* while loop, relational <= (Unit II, I) */
        mid = low + (high - low) / 2;              /* arithmetic +, -, / (Unit I)             */

        if ((list + mid)->roll_no == roll_no) {     /* pointer notation (Unit V)               */
            return mid;                             /* found! */
        }
        else if ((list + mid)->roll_no < roll_no) {
            low = mid + 1;
        }
        else {
            high = mid - 1;                         /* arithmetic - (Unit I) */
        }
    }

    return -1;  /* not found */
}

/* ═══════════════════════════════════════════════════════════════════════════
 *  FUNCTION: sort_bubble
 *
 *  Purpose : Sorts the student array by CGPA (descending) using bubble
 *            sort algorithm. Uses 'register' keyword for loop variable.
 *  Params  : list  — pointer to student array
 *            count — number of students
 *  Returns : void
 *  Covers  : Unit IV (bubble sort, in-place), Unit III (register keyword)
 * ═══════════════════════════════════════════════════════════════════════════ */
void sort_bubble(Student *list, int count)
{
    register int i, j;      /* register storage class for loop variables (Unit III) */
    Student temp;
    int swapped;

    printf("  Sorting %d students by CGPA (descending)...\n", count);

    for (i = 0; i < count - 1; i++) {
        swapped = 0;
        for (j = 0; j < count - 1 - i; j++) {      /* arithmetic -, * implicit in array (Unit I) */
            /* Sort descending: swap if current CGPA < next CGPA */
            if ((list + j)->cgpa < (list + j + 1)->cgpa) {  /* relational < (Unit I) */
                /* Swap entire student structs */
                temp = *(list + j);                 /* dereference * (Unit I, V) */
                *(list + j) = *(list + j + 1);
                *(list + j + 1) = temp;
                swapped = 1;
            }
        }
        /* Optimization: if no swaps occurred, array is sorted */
        if (!swapped) {                             /* logical NOT ! (Unit I) */
            break;  /* break — exit early if already sorted (Unit II) */
        }
    }

    printf("  Sort complete.\n");
}

/* ═══════════════════════════════════════════════════════════════════════════
 *  FUNCTION: update_student
 *
 *  Purpose : Updates a student's record by roll number. Demonstrates
 *            calloc() to create a temporary zero-initialized copy (Unit V).
 *  Params  : list  — pointer to student array
 *            count — number of students
 *  Returns : void
 *  Covers  : Unit V (calloc, pointer notation), Unit II (I/O)
 * ═══════════════════════════════════════════════════════════════════════════ */
void update_student(Student *list, int count)
{
    int roll, i, found_idx = -1;
    int choice;
    Student *ptr;
    Student *temp_copy = NULL;
    float sum, mark;

    printf("\n%s\n", LINE_SEP);
    printf("  UPDATE STUDENT RECORD\n");
    printf("%s\n\n", LINE_SEP);

    if (count == 0) {
        printf("  No students to update.\n");
        return;
    }

    printf("  Enter Roll Number to update: ");
    scanf("%d", &roll);
    flush_input();

    /* Find the student */
    for (i = 0; i < count; i++) {
        if ((list + i)->roll_no == roll) {
            found_idx = i;
            break;              /* break (Unit II) */
        }
    }

    if (found_idx == -1) {      /* relational == (Unit I) */
        printf(COLOR_RED "  [Error] Student with Roll No %d not found.\n" COLOR_RESET, roll);
        return;
    }

    ptr = list + found_idx;

    /* ── calloc() — create a temporary zero-initialized copy (Unit V) ── */
    temp_copy = (Student *)calloc(1, sizeof(Student));
    if (temp_copy == NULL) {
        fprintf(stderr, "  [Error] calloc failed for temporary copy.\n");
        return;
    }
    *temp_copy = *ptr;  /* copy current data into temp */
    printf("  [Memory] Temporary copy created using calloc() — all bytes zero-initialized.\n");

    printf("\n  Current record for Roll No %d:\n", roll);
    printf("    Name: %s | Age: %d | CGPA: %.2f | Dept: %s\n",
           ptr->name, ptr->age, ptr->cgpa, ptr->department);

    printf("\n  What do you want to update?\n");
    printf("    1. Name\n");
    printf("    2. Age\n");
    printf("    3. Department\n");
    printf("    4. City\n");
    printf("    5. State\n");
    printf("    6. Subject Marks (recalculates CGPA & Grade)\n");
    printf("  Choice: ");
    scanf("%d", &choice);
    flush_input();

    switch (choice) {           /* switch-case (Unit II) */
        case 1:
            printf("  Enter New Name: ");
            fgets(ptr->name, MAX_NAME, stdin);
            {
                size_t len = strlen(ptr->name);
                if (len > 0 && ptr->name[len - 1] == '\n')
                    ptr->name[len - 1] = '\0';
            }
            break;
        case 2:
            printf("  Enter New Age: ");
            scanf("%d", &ptr->age);
            flush_input();
            break;
        case 3:
            printf("  Enter New Department: ");
            fgets(ptr->department, MAX_DEPT, stdin);
            {
                size_t len = strlen(ptr->department);
                if (len > 0 && ptr->department[len - 1] == '\n')
                    ptr->department[len - 1] = '\0';
            }
            break;
        case 4:
            printf("  Enter New City: ");
            fgets(ptr->address.city, MAX_CITY, stdin);
            {
                size_t len = strlen(ptr->address.city);
                if (len > 0 && ptr->address.city[len - 1] == '\n')
                    ptr->address.city[len - 1] = '\0';
            }
            break;
        case 5:
            printf("  Enter New State: ");
            fgets(ptr->address.state, MAX_STATE, stdin);
            {
                size_t len = strlen(ptr->address.state);
                if (len > 0 && ptr->address.state[len - 1] == '\n')
                    ptr->address.state[len - 1] = '\0';
            }
            break;
        case 6:
            printf("  Enter new marks for %d subjects (0-100):\n", NUM_SUBJECTS);
            sum = 0.0f;
            for (i = 0; i < NUM_SUBJECTS; i++) {
                do {
                    printf("    Subject %d: ", i + 1);
                    scanf("%f", &mark);
                    flush_input();
                } while (mark < 0.0f || mark > 100.0f);
                ptr->marks.subjects[i] = mark;
                sum += mark;
            }
            ptr->cgpa = (sum / NUM_SUBJECTS) / 10.0f;
            ptr->cgpa = (float)((int)(ptr->cgpa * 100 + 0.5f)) / 100.0f; /* Fix 16 */
            ptr->result.grade = calculate_grade(ptr->cgpa);
            printf("  Updated CGPA: %.2f | Grade: %c\n", ptr->cgpa, ptr->result.grade);
            break;
        default:
            printf("  Invalid choice. No changes made.\n");
            /* Restore from calloc'd backup */
            *ptr = *temp_copy;
            /* Fix 8: free temp_copy before returning */
            if (temp_copy != NULL) {
                free(temp_copy);
                temp_copy = NULL;
            }
            return;
    }

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

    printf(COLOR_GREEN "  [Success] Student record updated.\n" COLOR_RESET);

    /* Auto-save to file (Upgrade 1) */
    save_to_file(list, count);
}

/* ═══════════════════════════════════════════════════════════════════════════
 *  FUNCTION: delete_student
 *
 *  Purpose : Deletes a student by roll number and shifts remaining elements
 *            left to fill the gap (Unit IV — element shifting).
 *  Params  : list  — pointer to student array pointer
 *            count — pointer to current student count
 *  Returns : void
 *  Covers  : Unit IV (delete with shift), Unit V (pointer arithmetic)
 * ═══════════════════════════════════════════════════════════════════════════ */
void delete_student(Student **list, int *count)
{
    int roll, i, found_idx = -1;

    printf("\n%s\n", LINE_SEP);
    printf("  DELETE STUDENT RECORD\n");
    printf("%s\n\n", LINE_SEP);

    if (*count == 0) {
        printf("  No students to delete.\n");
        return;
    }

    printf("  Enter Roll Number to delete: ");
    scanf("%d", &roll);
    flush_input();

    /* Find the student */
    for (i = 0; i < *count; i++) {
        if ((*list + i)->roll_no == roll) {
            found_idx = i;
            break;
        }
    }

    if (found_idx == -1) {
        printf(COLOR_RED "  [Error] Student with Roll No %d not found.\n" COLOR_RESET, roll);
        return;
    }

    printf("  Deleting '%s' (Roll #%d)...\n",
           (*list + found_idx)->name, (*list + found_idx)->roll_no);

    char confirm;
    printf(COLOR_YELLOW
        "  Are you sure you want to delete '%s' "
        "(Roll #%d)? (y/n): " COLOR_RESET,
        (*list + found_idx)->name,
        (*list + found_idx)->roll_no);
    scanf(" %c", &confirm);
    flush_input();
    if (confirm != 'y' && confirm != 'Y') {
        printf("  Deletion cancelled.\n");
        return;
    }

    /* ── Element shifting: move all elements after found_idx one position left (Unit IV) ── */
    for (i = found_idx; i < *count - 1; i++) {
        *(*list + i) = *(*list + i + 1);    /* shift left — pointer arithmetic (Unit V) */
    }

    (*count)--;     /* decrement count — unary -- (Unit I) */

    printf(COLOR_GREEN "  [Success] Student deleted. %d students remaining.\n" COLOR_RESET, *count);

    /* Auto-save to file (Upgrade 1) */
    save_to_file(*list, *count);
}

/* ═══════════════════════════════════════════════════════════════════════════
 *  FUNCTION: display_statistics
 *
 *  Purpose : Shows average, highest, and lowest CGPA across all students.
 *            Uses 'double' data type for precise average (Unit I).
 *            Demonstrates ternary operator and modulo (%) operator.
 *  Params  : list  — pointer to student array
 *            count — number of students
 *  Returns : void
 *  Covers  : Unit I (double, ternary, arithmetic %), Unit II (for loop)
 * ═══════════════════════════════════════════════════════════════════════════ */
void display_statistics(Student *list, int count)
{
    double sum_cgpa = 0.0;      /* double data type for precision (Unit I) */
    double avg_cgpa;
    float highest = 0.0f;
    float lowest = 10.0f;
    int highest_idx = 0, lowest_idx = 0;
    int i;
    int pass_count = 0, fail_count = 0;

    printf("\n%s\n", LINE_SEP);
    printf(COLOR_MAGENTA COLOR_BOLD "  STATISTICS — CGPA ANALYSIS\n" COLOR_RESET);
    printf("%s\n\n", LINE_SEP);

    if (count == 0) {
        printf("  No students to analyze.\n");
        return;
    }

    for (i = 0; i < count; i++) {
        sum_cgpa += (double)((list + i)->cgpa);     /* cast to double (Unit I) */

        if ((list + i)->cgpa > highest) {           /* relational > (Unit I) */
            highest = (list + i)->cgpa;
            highest_idx = i;
        }
        if ((list + i)->cgpa < lowest) {
            lowest = (list + i)->cgpa;
            lowest_idx = i;
        }

        /* Count pass/fail using ternary + assignment (Unit I) */
        ((list + i)->cgpa >= 5.0f) ? pass_count++ : fail_count++;
    }

    avg_cgpa = sum_cgpa / count;    /* double division for precision */

    printf("  ┌─────────────────────────────────────────────────┐\n");
    printf("  │  Total Students    : %-5d                      │\n", count);
    printf("  │  Average CGPA      : %-8.4f                   │\n", avg_cgpa);
    printf("  │  Highest CGPA      : %-7.2f  (%-15s)  │\n", highest, (list + highest_idx)->name);
    printf("  │  Lowest CGPA       : %-7.2f  (%-15s)  │\n", lowest, (list + lowest_idx)->name);
    printf("  │  Pass (CGPA >= 5)  : %-5d                      │\n", pass_count);
    printf("  │  Fail (CGPA < 5)   : %-5d                      │\n", fail_count);
    printf("  │  Pass Rate         : %-6.1f%%                    │\n",
           (count > 0) ? (pass_count * 100.0 / count) : 0.0);
    printf("  └─────────────────────────────────────────────────┘\n");

    /* Modulo operator demonstration (Unit I — arithmetic %) */
    printf("\n  [Note] Student count is %s (count %% 2 = %d)\n",
           (count % 2 == 0) ? "EVEN" : "ODD", count % 2);
}

/* ═══════════════════════════════════════════════════════════════════════════
 *  FUNCTION: demo_storage_classes
 *
 *  Purpose : Demonstrates all 4 storage classes: auto, static, extern,
 *            register. Also demonstrates all remaining data types (short,
 *            long, double) and all 7 operator types including bitwise.
 *  Params  : None
 *  Returns : void
 *  Covers  : Unit III (storage classes), Unit I (all data types & operators)
 * ═══════════════════════════════════════════════════════════════════════════ */
void demo_storage_classes(void)
{
    /* ── auto storage class (Unit III) ── */
    auto int auto_var = 42;     /* 'auto' explicitly declared — default for locals */

    /* ── static storage class (Unit III) ── */
    static int call_count = 0;  /* persists across calls; counts how many times invoked */
    call_count++;

    /* ── extern storage class (Unit III) ── */
    extern int total_students;  /* references the global variable */

    /* ── register storage class (Unit III) ── */
    register int reg_i;         /* hint to store in CPU register for speed */

    /* ── ALL remaining data types (Unit I) ── */
    short  short_val  = 16;     /* short data type   */
    long   long_val   = 1000000L;  /* long data type */
    double double_val = 3.14159265358979;  /* double data type */
    char   char_val   = 'X';   /* char data type    */

    printf("\n%s\n", LINE_SEP);
    printf("  DEMO: STORAGE CLASSES & DATA TYPES\n");
    printf("%s\n\n", LINE_SEP);

    printf("  ╔═══════════════════════════════════════════════════════════╗\n");
    printf("  ║  STORAGE CLASSES (Unit III)                              ║\n");
    printf("  ╠═══════════════════════════════════════════════════════════╣\n");
    printf("  ║  auto     : auto_var = %-6d  (default for local vars)  ║\n", auto_var);
    printf("  ║  static   : call_count = %-3d  (this is call #%d)        ║\n", call_count, call_count);
    printf("  ║  extern   : total_students = %-3d (global variable)      ║\n", total_students);
    printf("  ║  register : reg_i is a register-hint loop variable       ║\n");
    printf("  ╚═══════════════════════════════════════════════════════════╝\n");

    /* Using register variable in a loop */
    printf("\n  Register variable loop (1 to 5): ");
    for (reg_i = 1; reg_i <= 5; reg_i++) {
        printf("%d ", reg_i);
    }
    printf("\n");

    printf("\n  ╔═══════════════════════════════════════════════════════════╗\n");
    printf("  ║  ALL C DATA TYPES (Unit I)                               ║\n");
    printf("  ╠═══════════════════════════════════════════════════════════╣\n");
    printf("  ║  int    : auto_var    = %-10d                        ║\n", auto_var);
    printf("  ║  float  : (CGPA)     = %-10.2f                        ║\n", 8.66f);
    printf("  ║  double : double_val = %-15.10f                   ║\n", double_val);
    printf("  ║  char   : char_val   = '%c'  (ASCII: %d)                 ║\n", char_val, char_val);
    printf("  ║  short  : short_val  = %-10hd                        ║\n", short_val);
    printf("  ║  long   : long_val   = %-10ld                        ║\n", long_val);
    printf("  ╚═══════════════════════════════════════════════════════════╝\n");

    /* ═══════════════════════════════════════════════════════════════════
     *  OPERATOR TYPE DEMONSTRATIONS (Unit I)
     *  All 7 operator types demonstrated with clear examples:
     * ═══════════════════════════════════════════════════════════════════ */
    {
        int a = 12, b = 10, result;

        printf("\n  ╔═══════════════════════════════════════════════════════════╗\n");
        printf("  ║  ALL 7 OPERATOR TYPES (Unit I)                           ║\n");
        printf("  ╠═══════════════════════════════════════════════════════════╣\n");

        /* 1. Arithmetic operators: +, -, *, /, % */
        printf("  ║  1. ARITHMETIC:  a=12, b=10                              ║\n");
        printf("  ║     a + b  = %-4d    a - b  = %-4d                       ║\n", a + b, a - b);
        printf("  ║     a * b  = %-4d   a / b  = %-4d   a %% b = %-4d        ║\n", a * b, a / b, a % b);

        /* 2. Unary operators: ++, --, !, -(negation), &, * */
        result = a++;       /* post-increment */
        printf("  ║  2. UNARY:                                               ║\n");
        printf("  ║     a++ = %-4d (post)    ++a = %-4d (pre)                ║\n", result, ++a);
        --a; --a;           /* restore a back to 12 */
        printf("  ║     -a  = %-4d           !0  = %-4d                      ║\n", -a, !0);
        printf("  ║     &a  = address-of operator (printed below)    ║\n");

        /* 3. Relational operators: <, >, <=, >=, ==, != */
        printf("  ║  3. RELATIONAL:  a=12, b=10                              ║\n");
        printf("  ║     a > b  = %d   a < b  = %d   a == b = %d              ║\n", a > b, a < b, a == b);
        printf("  ║     a >= b = %d   a <= b = %d   a != b = %d              ║\n", a >= b, a <= b, a != b);

        /* 4. Logical operators: &&, ||, ! */
        printf("  ║  4. LOGICAL:                                             ║\n");
        printf("  ║     (a>0) && (b>0) = %d                                  ║\n", (a > 0) && (b > 0));
        printf("  ║     (a>0) || (b<0) = %d                                  ║\n", (a > 0) || (b < 0));
        printf("  ║     !(a == b)      = %d                                  ║\n", !(a == b));

        /* 5. Assignment operators: =, +=, -=, *=, /=, %= */
        result = a;         /* simple assignment */
        result += 5;        /* compound assignment += */
        result -= 2;        /* compound assignment -= */
        result *= 2;        /* compound assignment *= */
        printf("  ║  5. ASSIGNMENT:  result = a                              ║\n");
        printf("  ║     result += 5 -= 2 *= 2 = %-4d                        ║\n", result);

        /* 6. Conditional (ternary) operator: ? : */
        printf("  ║  6. TERNARY:                                             ║\n");
        printf("  ║     a > b ? \"a wins\" : \"b wins\" = %s                    ║\n",
               a > b ? "a wins" : "b wins");

        /* 7. Bitwise operators: &, |, ^, ~, <<, >> */
        printf("  ║  7. BITWISE:  a=12 (1100), b=10 (1010)                  ║\n");
        printf("  ║     a & b  = %-3d  (AND)                                 ║\n", a & b);
        printf("  ║     a | b  = %-3d  (OR)                                  ║\n", a | b);
        printf("  ║     a ^ b  = %-3d  (XOR)                                 ║\n", a ^ b);
        printf("  ║     ~a     = %-5d (NOT)                                  ║\n", ~a);
        printf("  ║     a << 1 = %-3d  (Left Shift)                          ║\n", a << 1);
        printf("  ║     a >> 1 = %-3d  (Right Shift)                         ║\n", a >> 1);

        printf("  ╚═══════════════════════════════════════════════════════════╝\n");
        printf("  (Unary &) Address of variable a: %p\n", (void *)&a);
    }
}

/* ═══════════════════════════════════════════════════════════════════════════
 *  FUNCTION: swap_by_value
 *
 *  Purpose : Demonstrates call by value — swaps are LOCAL only.
 *            Changes do NOT reflect in the calling function.
 *  Params  : a, b — copies of integers (passed by value)
 *  Returns : void
 *  Covers  : Unit III (call by value)
 * ═══════════════════════════════════════════════════════════════════════════ */
void swap_by_value(int a, int b)
{
    int temp = a;
    a = b;
    b = temp;
    printf("    Inside swap_by_value  : a = %d, b = %d  (swapped LOCALLY)\n", a, b);
}

/* ═══════════════════════════════════════════════════════════════════════════
 *  FUNCTION: swap_by_address
 *
 *  Purpose : Demonstrates call by address (reference) — swaps are PERMANENT.
 *            Changes reflect in the calling function.
 *  Params  : a, b — pointers to integers (passed by address)
 *  Returns : void
 *  Covers  : Unit III (call by address), Unit V (pointer dereference)
 * ═══════════════════════════════════════════════════════════════════════════ */
void swap_by_address(int *a, int *b)
{
    int temp = *a;          /* dereference pointer — unary * (Unit I, V) */
    *a = *b;
    *b = temp;
    printf("    Inside swap_by_address: a = %d, b = %d  (swapped via POINTERS)\n", *a, *b);
}

/* ═══════════════════════════════════════════════════════════════════════════
 *  FUNCTION: demo_call_types
 *
 *  Purpose : Demonstrates call by value vs. call by address with a clear
 *            before-and-after comparison.
 *  Params  : None
 *  Returns : void
 *  Covers  : Unit III (function calling mechanisms)
 * ═══════════════════════════════════════════════════════════════════════════ */
void demo_call_types(void)
{
    int x = 10, y = 20;

    printf("\n%s\n", LINE_SEP);
    printf("  DEMO: CALL BY VALUE vs CALL BY ADDRESS (Unit III)\n");
    printf("%s\n\n", LINE_SEP);

    /* ── Call by Value ── */
    printf("  ┌─── CALL BY VALUE ─────────────────────────────────────────┐\n");
    printf("  │  Before swap_by_value : x = %d, y = %d                   │\n", x, y);
    swap_by_value(x, y);
    printf("  │  After swap_by_value  : x = %d, y = %d  (UNCHANGED!)     │\n", x, y);
    printf("  │  Explanation: Function receives COPIES — originals safe.  │\n");
    printf("  └───────────────────────────────────────────────────────────┘\n");

    printf("\n");

    /* ── Call by Address ── */
    printf("  ┌─── CALL BY ADDRESS (REFERENCE) ──────────────────────────┐\n");
    printf("  │  Before swap_by_address: x = %d, y = %d                  │\n", x, y);
    swap_by_address(&x, &y);   /* pass addresses — unary & (Unit I) */
    printf("  │  After swap_by_address : x = %d, y = %d  (SWAPPED!)     │\n", x, y);
    printf("  │  Explanation: Function receives ADDRESSES — modifies     │\n");
    printf("  │               originals through pointer dereference.     │\n");
    printf("  └───────────────────────────────────────────────────────────┘\n");
}

/* ═══════════════════════════════════════════════════════════════════════════
 *  FUNCTION: demo_pointer_arithmetic
 *
 *  Purpose : Demonstrates pointer arithmetic on the student array:
 *            - Array name is a pointer
 *            - Pointer + offset to access elements
 *            - Void pointer (generic pointer) usage
 *            - Address differences
 *  Params  : list  — pointer to student array
 *            count — number of students
 *  Returns : void
 *  Covers  : Unit V (pointer arithmetic, void pointer, array-as-pointer)
 * ═══════════════════════════════════════════════════════════════════════════ */
void demo_pointer_arithmetic(Student *list, int count)
{
    Student *ptr;
    int i;
    void *generic_ptr;          /* void pointer (generic pointer) — Unit V */

    printf("\n%s\n", LINE_SEP);
    printf("  DEMO: POINTER ARITHMETIC (Unit V)\n");
    printf("%s\n\n", LINE_SEP);

    if (count == 0) {
        printf("  No students to demonstrate.\n");
        return;
    }

    /* ── Demonstration: array name IS a pointer ── */
    printf("  ┌─── ARRAY NAME IS A POINTER ──────────────────────────────────────┐\n");
    printf("  │  'list' (array name) = %p                          │\n", (void *)list);
    printf("  │  '&list[0]'          = %p                          │\n", (void *)&list[0]);
    printf("  │  Both are EQUAL — proving array name = pointer to first element. │\n");
    printf("  └──────────────────────────────────────────────────────────────────┘\n\n");

    /* ── Pointer arithmetic: traversing array ── */
    printf("  ┌─── POINTER ARITHMETIC — TRAVERSING STUDENT ARRAY ───────────────┐\n");
    printf("  │  sizeof(Student) = %lu bytes                                     │\n",
           (unsigned long)sizeof(Student));
    printf("  │                                                                  │\n");

    for (i = 0; i < count; i++) {
        ptr = list + i;     /* pointer arithmetic: list + offset (Unit V) */
        printf("  │  list + %d = %p => Roll: %-4d Name: %-15s │\n",
               i, (void *)ptr, ptr->roll_no, ptr->name);
    }

    printf("  │                                                                  │\n");
    printf("  │  Address difference between list[0] and list[1]:                 │\n");
    if (count >= 2) {
        printf("  │  (list+1) - (list+0) = %ld element(s) = %lu bytes             │\n",
               (long)((list + 1) - list),
               (unsigned long)(sizeof(Student)));
    }
    printf("  └──────────────────────────────────────────────────────────────────┘\n\n");

    /* ── Void pointer (generic pointer) demonstration (Unit V) ── */
    generic_ptr = (void *)list;     /* assign Student* to void* — loses type info */
    printf("  ┌─── VOID POINTER (GENERIC POINTER) ──────────────────────────────┐\n");
    printf("  │  void *generic_ptr = (void *)list;                              │\n");
    printf("  │  generic_ptr  = %p                                │\n", generic_ptr);
    printf("  │                                                                  │\n");
    printf("  │  To use void pointer, MUST cast back to original type:           │\n");
    {
        Student *typed_ptr = (Student *)generic_ptr;    /* cast void* back */
        printf("  │  Student *typed = (Student *)generic_ptr;                       │\n");
        printf("  │  typed->name = \"%s\"                                      │\n",
               typed_ptr->name);
        printf("  │  typed->roll_no = %d                                            │\n",
               typed_ptr->roll_no);
    }
    printf("  │                                                                  │\n");
    printf("  │  Void pointers cannot be dereferenced directly — they need       │\n");
    printf("  │  a cast to a specific type first. Used in generic functions       │\n");
    printf("  │  like malloc(), qsort(), etc.                                    │\n");
    printf("  └──────────────────────────────────────────────────────────────────┘\n");
}

/* ═══════════════════════════════════════════════════════════════════════════
 *  FUNCTION: save_to_file (Upgrade 1 — File I/O Persistence)
 *
 *  Purpose : Saves all student records to a binary file using fwrite().
 *            Writes student count as first 4 bytes, then all struct data.
 *  Params  : arr   — pointer to student array
 *            count — number of students
 *  Returns : void
 *  Covers  : File I/O (fopen, fwrite, fclose)
 * ═══════════════════════════════════════════════════════════════════════════ */
void save_to_file(Student *arr, int count)
{
    FILE *fp;

    fp = fopen(FILE_NAME, "wb");    /* binary write mode */
    if (fp == NULL) {
        printf(COLOR_RED "  [Error] Could not open '%s' for writing.\n" COLOR_RESET, FILE_NAME);
        return;
    }

    /* Write count as first 4 bytes */
    fwrite(&count, sizeof(int), 1, fp);

    /* Write all student structs sequentially */
    fwrite(arr, sizeof(Student), count, fp);

    fclose(fp);
    printf(COLOR_GREEN "  [File] Data saved to '%s' (%d students).\n" COLOR_RESET, FILE_NAME, count);
}

/* ═══════════════════════════════════════════════════════════════════════════
 *  FUNCTION: load_from_file (Upgrade 1 — File I/O Persistence)
 *
 *  Purpose : Loads student records from binary file using fread().
 *            Reads student count first, then allocates exact memory.
 *  Params  : arr      — pointer to student array pointer
 *            count    — pointer to student count
 *            capacity — pointer to allocated capacity
 *  Returns : void
 *  Covers  : File I/O (fopen, fread, fclose), Dynamic memory (malloc)
 * ═══════════════════════════════════════════════════════════════════════════ */
void load_from_file(Student **arr, int *count, int *capacity)
{
    FILE *fp;
    int file_count;

    fp = fopen(FILE_NAME, "rb");    /* binary read mode */
    if (fp == NULL) {
        /* File doesn't exist on first run — this is normal */
        *count = 0;
        return;
    }

    /* Read student count from first 4 bytes */
    if (fread(&file_count, sizeof(int), 1, fp) != 1) {
        printf(COLOR_RED "  [Error] Could not read student count from file.\n" COLOR_RESET);
        fclose(fp);
        *count = 0;
        return;
    }

    if (file_count <= 0) {
        fclose(fp);
        *count = 0;
        return;
    }

    /* Free old memory and allocate exact amount needed */
    free(*arr);
    *arr = (Student *)malloc(file_count * sizeof(Student));
    if (*arr == NULL) {
        printf(COLOR_RED "  [FATAL] Memory allocation failed during file load.\n" COLOR_RESET);
        fclose(fp);
        exit(1);
    }

    /* Read all student structs */
    if ((int)fread(*arr, sizeof(Student), file_count, fp) != file_count) {
        printf(COLOR_RED "  [Error] Could not read all student records from file.\n" COLOR_RESET);
        fclose(fp);
        *count = 0;
        return;
    }

    *count = file_count;
    *capacity = file_count;
    fclose(fp);
}

/* ═══════════════════════════════════════════════════════════════════════════
 *  FUNCTION: is_valid_input (Upgrade 3 — Input Validation)
 *
 *  Purpose : Validates a Student struct's fields. Returns 1 if valid, 0 if not.
 *            Uses a static variable to track total failed validation attempts.
 *  Params  : s         — pointer to student to validate
 *            list      — pointer to full student array (for duplicate check)
 *            count     — current number of students
 *            self_roll — roll number to skip in duplicate check (-1 for new)
 *  Returns : int — 1 if all fields valid, 0 otherwise
 *  Covers  : Unit III (static variable), Unit V (string traversal with isalpha)
 * ═══════════════════════════════════════════════════════════════════════════ */
int is_valid_input(Student *s, Student *list, int count, int self_roll)
{
    static int session_fail_count = 0; /* static: tracks total failures this session */
    int i;
    int valid = 1;                  /* assume valid until proven otherwise */

    /* Rule 1: roll_no must be > 0 and <= 9999 */
    if (s->roll_no <= 0 || s->roll_no > 9999) {
        printf(COLOR_RED "  [Validation] Roll number must be between 1 and 9999.\n" COLOR_RESET);
        valid = 0;
    }

    /* Rule 2: duplicate roll_no check (skip self_roll on update) */
    for (i = 0; i < count; i++) {
        if ((list + i)->roll_no == s->roll_no && s->roll_no != self_roll) {
            printf(COLOR_RED "  [Validation] Roll number %d already exists.\n" COLOR_RESET, s->roll_no);
            valid = 0;
            break;
        }
    }

    /* Rule 3: name must be >= 2 chars and only alphabets/spaces */
    if (strlen(s->name) < 2) {
        printf(COLOR_RED "  [Validation] Name must be at least 2 characters.\n" COLOR_RESET);
        valid = 0;
    } else {
        for (i = 0; s->name[i] != '\0'; i++) {
            if (!isalpha((unsigned char)s->name[i]) && !isspace((unsigned char)s->name[i])) {
                printf(COLOR_RED "  [Validation] Name must contain only alphabets and spaces.\n" COLOR_RESET);
                valid = 0;
                break;
            }
        }
    }

    /* Rule 4: age must be 16-35 */
    if (s->age < 16 || s->age > 35) {
        printf(COLOR_RED "  [Validation] Age must be between 16 and 35.\n" COLOR_RESET);
        valid = 0;
    }

    /* Rule 5: marks must be 0-100 each */
    for (i = 0; i < NUM_SUBJECTS; i++) {
        if (s->marks.subjects[i] < 0.0f || s->marks.subjects[i] > 100.0f) {
            printf(COLOR_RED "  [Validation] Subject %d mark must be between 0 and 100.\n" COLOR_RESET, i + 1);
            valid = 0;
        }
    }

    /* Rule 6: department must not be empty */
    if (strlen(s->department) == 0) {
        printf(COLOR_RED "  [Validation] Department must not be empty.\n" COLOR_RESET);
        valid = 0;
    }

    /* Rule 7: city and state must not be empty */
    if (strlen(s->address.city) == 0) {
        printf(COLOR_RED "  [Validation] City must not be empty.\n" COLOR_RESET);
        valid = 0;
    }
    if (strlen(s->address.state) == 0) {
        printf(COLOR_RED "  [Validation] State must not be empty.\n" COLOR_RESET);
        valid = 0;
    }

    if (!valid) {
        session_fail_count++;
        printf(COLOR_YELLOW
            "  [Validation] Total validation failures "
            "this session: %d\n" COLOR_RESET, session_fail_count);
    }

    return valid;
}

/* ═══════════════════════════════════════════════════════════════════════════
 *  FUNCTION: export_report (Upgrade 4 — Text Report Export)
 *
 *  Purpose : Exports a complete formatted report to a text file.
 *            Uses time.h for timestamp generation.
 *  Params  : arr   — pointer to student array
 *            count — number of students
 *  Returns : void
 *  Covers  : File I/O (fprintf, fopen, fclose), time.h (time, localtime, strftime)
 * ═══════════════════════════════════════════════════════════════════════════ */
void export_report(Student *arr, int count)
{
    FILE *fp;
    char filename[100];
    char timestamp[80];
    time_t now;
    struct tm *local_time;
    int i, j;
    float highest = 0.0f, lowest = 10.0f;
    int highest_idx = 0, lowest_idx = 0;
    double sum_cgpa = 0.0;
    int pass_count = 0, fail_count = 0;
    float row_avg;

    /* Ask user for filename */
    printf("\n  Enter output filename (press Enter for 'srms_report.txt'): ");
    fgets(filename, sizeof(filename), stdin);
    {
        size_t len = strlen(filename);
        if (len > 0 && filename[len - 1] == '\n')
            filename[len - 1] = '\0';
    }

    /* Default filename if empty */
    if (strlen(filename) == 0) {
        strcpy(filename, "srms_report.txt");
    }

    fp = fopen(filename, "w");
    if (fp == NULL) {
        printf(COLOR_RED "  [Error] Could not create '%s'.\n" COLOR_RESET, filename);
        return;
    }

    /* Get current timestamp using time.h */
    now = time(NULL);                           /* time() usage */
    local_time = localtime(&now);               /* localtime() usage */
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", local_time);  /* strftime() usage */

    /* ── Report Header ── */
    fprintf(fp, "================================================\n");
    fprintf(fp, "  STUDENT RECORD MANAGEMENT SYSTEM — REPORT\n");
    fprintf(fp, "  Generated on: %s\n", timestamp);
    fprintf(fp, "  Total Students: %d\n", count);
    fprintf(fp, "================================================\n\n");

    /* ── Student Table ── */
    fprintf(fp, "  ROLL |  NAME              | AGE | DEPT         | CGPA  | GRADE\n");
    fprintf(fp, "  -----|--------------------|----|--------------|-------|------\n");
    for (i = 0; i < count; i++) {
        fprintf(fp, "  %-4d |  %-18s | %-3d | %-12s | %-5.2f |  %c\n",
                (arr + i)->roll_no,
                (arr + i)->name,
                (arr + i)->age,
                (arr + i)->department,
                (arr + i)->cgpa,
                (arr + i)->result.grade);
    }

    /* ── Subject-wise Marks ── */
    fprintf(fp, "\n  SUBJECT-WISE MARKS:\n");
    fprintf(fp, "  ROLL | SUB1  | SUB2  | SUB3  | SUB4  | SUB5  | AVG\n");
    fprintf(fp, "  -----|-------|-------|-------|-------|-------|-------\n");
    for (i = 0; i < count; i++) {
        row_avg = 0.0f;
        fprintf(fp, "  %-4d ", (arr + i)->roll_no);
        for (j = 0; j < NUM_SUBJECTS; j++) {
            fprintf(fp, "| %-5.1f ", (arr + i)->marks.subjects[j]);
            row_avg += (arr + i)->marks.subjects[j];
        }
        fprintf(fp, "| %-5.1f\n", row_avg / NUM_SUBJECTS);
    }

    /* ── Compute Statistics ── */
    for (i = 0; i < count; i++) {
        sum_cgpa += (double)((arr + i)->cgpa);
        if ((arr + i)->cgpa > highest) { highest = (arr + i)->cgpa; highest_idx = i; }
        if ((arr + i)->cgpa < lowest)  { lowest  = (arr + i)->cgpa; lowest_idx  = i; }
        ((arr + i)->cgpa >= 5.0f) ? pass_count++ : fail_count++;
    }

    fprintf(fp, "\n  STATISTICS:\n");
    if (count > 0) {
        fprintf(fp, "  Highest CGPA: %.2f (Name: %s)\n", highest, (arr + highest_idx)->name);
        fprintf(fp, "  Lowest  CGPA: %.2f (Name: %s)\n", lowest, (arr + lowest_idx)->name);
        fprintf(fp, "  Average CGPA: %.2f\n", sum_cgpa / count);
    }
    fprintf(fp, "  Pass Count  : %d\n", pass_count);
    fprintf(fp, "  Fail Count  : %d\n", fail_count);

    fclose(fp);

    printf(COLOR_GREEN "  Report exported to '%s'.\n" COLOR_RESET, filename);
}

/* ═══════════════════════════════════════════════════════════════════════════
 *  FUNCTION: recursive_menu
 *
 *  Purpose : Displays the main menu and handles user input. Calls itself
 *            recursively after each operation (base case: Exit).
 *            Contains a static operations_counter (Unit III).
 *            Expanded to 16 options with ANSI color theming.
 *  Params  : list     — pointer to student array pointer
 *            count    — pointer to current student count
 *            capacity — pointer to allocated capacity
 *  Returns : void
 *  Covers  : Unit II (switch-case, formatted I/O), Unit III (static, recursion)
 * ═══════════════════════════════════════════════════════════════════════════ */
void recursive_menu(Student **list, int *count, int *capacity)
{
    /* static variable: counts total operations in this session (Unit III) */
    static int operations_counter = 0;
    int choice;
    char search_name[MAX_NAME];
    int search_roll, result_idx;

    /* ── Print Menu ── */
    printf("\n  ╔══════════════════════════════════════════════════════╗\n");
    printf("  ║   " COLOR_BOLD "STUDENT RECORD MANAGEMENT SYSTEM" COLOR_RESET "                 ║\n");
    printf("  ║          CSE101 - C Project                         ║\n");
    printf("  ╚══════════════════════════════════════════════════════╝\n\n");

    printf(COLOR_YELLOW);
    printf("   1.  Add New Student\n");
    printf("   2.  Display All Students (Table View)\n");
    printf("   3.  Display Marks Matrix (2D Array View)\n");
    printf("   4.  Search Student by Name (Linear Search)\n");
    printf("   5.  Search Student by Roll No (Binary Search)\n");
    printf("   6.  Sort Students by CGPA (Bubble Sort)\n");
    printf("   7.  Update Student Record\n");
    printf("   8.  Delete Student Record\n");
    printf("   9.  Display Statistics (Avg/Max/Min CGPA)\n");
    printf("  10.  Demo: Storage Classes\n");
    printf("  11.  Demo: Call by Value vs Call by Reference\n");
    printf("  12.  Demo: Pointer Arithmetic\n");
    printf("  13.  Save to File Manually\n");
    printf("  14.  Load from File\n");
    printf("  15.  Export Report to Text File\n");
    printf("  16.  Exit and Free Memory\n");
    printf(COLOR_RESET);

    printf("\n  ─────────────────────────────────────────\n");
    printf("  Operations this session: %d\n", operations_counter);
    printf("  Students in system: %d / %d (capacity)\n", *count, *capacity);
    printf("  ─────────────────────────────────────────\n");

    printf(COLOR_BOLD "\n  Enter your choice (1-16): " COLOR_RESET);
    scanf("%d", &choice);       /* scanf() for numeric input (Unit II) */
    flush_input();

    operations_counter++;       /* static counter incremented (Unit III) */

    /* ── switch-case for menu dispatch (Unit II) ── */
    switch (choice) {

        case 1:     /* Add New Student */
            add_student(list, count, capacity);
            break;

        case 2:     /* Display All Students */
            display_all_students(*list, *count);
            break;

        case 3:     /* Display Marks Matrix */
            display_marks_matrix(*list, *count);
            break;

        case 4:     /* Search by Name — Linear Search */
            printf("\n  Enter student name to search: ");
            fgets(search_name, MAX_NAME, stdin);
            {
                size_t len = strlen(search_name);
                if (len > 0 && search_name[len - 1] == '\n')
                    search_name[len - 1] = '\0';
            }
            result_idx = search_linear(*list, *count, search_name);
            if (result_idx != -1) {
                Student *found = *list + result_idx;
                printf(COLOR_GREEN "\n  [Found] Student at index %d:\n" COLOR_RESET, result_idx);
                printf("    Roll: %d | Name: %s | CGPA: %.2f | Grade: %c | Dept: %s\n",
                       found->roll_no, found->name, found->cgpa,
                       found->result.grade, found->department);
                printf("    Location: %s, %s\n",
                       found->address.city, found->address.state);
            } else {
                printf(COLOR_RED "  [Not Found] No student named '%s'.\n" COLOR_RESET, search_name);
            }
            break;

        case 5:     /* Search by Roll No — Binary Search */
            printf("\n  Enter Roll Number to search: ");
            scanf("%d", &search_roll);
            flush_input();

            {
                char bsearch_confirm;
                printf(COLOR_YELLOW
                    "  [Warning] Binary search requires roll-number "
                    "order.\n  This will overwrite any CGPA sort "
                    "(Option 6). Proceed? (y/n): " COLOR_RESET);
                scanf(" %c", &bsearch_confirm);
                flush_input();
                if (bsearch_confirm != 'y' && bsearch_confirm != 'Y') {
                    printf("  Binary search cancelled.\n");
                    break;
                }
                sort_by_roll(*list, *count);
                printf("  [Info] Array sorted by Roll Number.\n");
            }

            result_idx = search_binary(*list, *count, search_roll);
            if (result_idx != -1) {
                Student *found = *list + result_idx;
                printf(COLOR_GREEN "\n  [Found] Student at index %d:\n" COLOR_RESET, result_idx);
                printf("    Roll: %d | Name: %s | CGPA: %.2f | Grade: %c | Dept: %s\n",
                       found->roll_no, found->name, found->cgpa,
                       found->result.grade, found->department);
                printf("    Location: %s, %s\n",
                       found->address.city, found->address.state);
            } else {
                printf(COLOR_RED "  [Not Found] No student with Roll No %d.\n" COLOR_RESET, search_roll);
            }
            break;

        case 6:     /* Sort by CGPA — Bubble Sort */
            sort_bubble(*list, *count);
            printf(COLOR_GREEN "  Students sorted by CGPA (highest first).\n" COLOR_RESET);
            display_all_students(*list, *count);
            break;

        case 7:     /* Update Student */
            update_student(*list, *count);
            break;

        case 8:     /* Delete Student */
            delete_student(list, count);
            break;

        case 9:     /* Display Statistics */
            display_statistics(*list, *count);
            break;

        case 10:    /* Demo: Storage Classes */
            demo_storage_classes();
            break;

        case 11:    /* Demo: Call Types */
            demo_call_types();
            break;

        case 12:    /* Demo: Pointer Arithmetic */
            demo_pointer_arithmetic(*list, *count);
            break;

        case 13:    /* Save to File Manually */
            save_to_file(*list, *count);
            break;

        case 14:    /* Load from File */
            load_from_file(list, count, capacity);
            if (*count > 0) {
                printf(COLOR_GREEN "  Loaded %d students from '%s'.\n" COLOR_RESET, *count, FILE_NAME);
            } else {
                printf(COLOR_YELLOW "  No data found in '%s'.\n" COLOR_RESET, FILE_NAME);
            }
            break;

        case 15:    /* Export Report to Text File */
            export_report(*list, *count);
            break;

        case 16:    /* Exit */
            printf("\n  Total operations performed this session: %d\n", operations_counter);
            printf("  Freeing all dynamic memory...\n");
            return;     /* BASE CASE — stop recursion */

        default:
            printf(COLOR_RED "  [Error] Invalid choice! Please enter 1-16.\n" COLOR_RESET);
            break;
    }

    /*
     * Recursion depth guard — prevents stack overflow.
     * Unit III: demonstrates recursion with a practical
     * safety limit. Beyond 500 ops, function still works
     * via a self-reset mechanism.
     */
    if (operations_counter < 500) {
        recursive_menu(list, count, capacity);
    } else {
        printf(COLOR_YELLOW
            "  [System] Session operation limit reached (500).\n"
            "  Saving data and restarting session counter.\n"
            COLOR_RESET);
        save_to_file(*list, *count);
        /* Reset static counter by calling fresh instance */
        operations_counter = 0;
        recursive_menu(list, count, capacity);
    }
}

/* ═══════════════════════════════════════════════════════════════════════════
 *  END OF SRMS.C
 *
 *  Compilation:  gcc -Wall -o srms srms.c
 *  Execution  :  ./srms   (Linux/Mac)  or  srms.exe  (Windows)
 *
 *  All Units Covered:
 *    Unit I   : Data types, operators (all 7 types), structs, unions
 *    Unit II  : switch-case, for/while/do-while, break/continue/goto, I/O
 *    Unit III : Functions (25+), prototypes, storage classes, recursion
 *    Unit IV  : Arrays, 2D display, linear search, binary search, bubble sort
 *    Unit V   : Pointers, strings (strlen/strcpy/strcmp/strcat/strtok),
 *               dynamic memory (malloc/realloc/calloc/free), void pointer
 *    Unit VI  : Nested structs, unions, typedef (C++ in separate file)
 *
 *  Upgrades:
 *    Upgrade 1: Binary file I/O (save_to_file, load_from_file)
 *    Upgrade 2: ANSI terminal colors
 *    Upgrade 3: Input validation (is_valid_input)
 *    Upgrade 4: Text report export (export_report with time.h)
 * ═══════════════════════════════════════════════════════════════════════════ */
