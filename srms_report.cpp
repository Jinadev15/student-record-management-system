/******************************************************************************
 *  ╔══════════════════════════════════════════════════════════════════════╗
 *  ║   SRMS — C++ Extension Module (Unit VI: Basics of C++)             ║
 *  ║   File: srms_report.cpp                                            ║
 *  ║   CSE101 — C Programming Project                                   ║
 *  ╚══════════════════════════════════════════════════════════════════════╝
 *
 *  Compilation: g++ -Wall -o srms_report srms_report.cpp
 *  Execution  : ./srms_report  (Linux/Mac)  or  srms_report.exe  (Windows)
 *
 *  This module demonstrates:
 *    - Classes with private/public access
 *    - Constructors
 *    - Static data members and static member functions
 *    - Inline and non-inline member functions
 *    - cin/cout usage (not scanf/printf)
 *    - Object creation and method calls
 *    - struct vs class difference
 *    - Procedural (C) vs OOP (C++) comparison
 ******************************************************************************/

#include <iostream>
#include <string>
#include <iomanip>

using namespace std;

/* ═══════════════════════════════════════════════════════════════════════════
 *  DIFFERENCE BETWEEN STRUCT AND CLASS IN C++ (Unit VI)
 * ═══════════════════════════════════════════════════════════════════════════
 *
 *  In C++, both 'struct' and 'class' can have member functions,
 *  constructors, destructors, and support inheritance.
 *
 *  The ONLY DIFFERENCE is the DEFAULT ACCESS SPECIFIER:
 *
 *    struct MyStruct {
 *        int x;       // PUBLIC by default
 *        void f();    // PUBLIC by default
 *    };
 *
 *    class MyClass {
 *        int x;       // PRIVATE by default
 *        void f();    // PRIVATE by default
 *    };
 *
 *  In C, struct can ONLY contain data (no functions, no access control).
 *  In C++, struct is essentially a class with default public access.
 *
 *  Convention:
 *    - Use 'struct' for plain data holders (POD — Plain Old Data)
 *    - Use 'class' for objects with behavior, invariants, encapsulation
 * ═══════════════════════════════════════════════════════════════════════════ */

/* ═══════════════════════════════════════════════════════════════════════════
 *  PROCEDURAL (C) vs OBJECT-ORIENTED (C++) APPROACH (Unit VI)
 * ═══════════════════════════════════════════════════════════════════════════
 *
 *  ┌──────────────────────────────────────────────────────────────────────┐
 *  │  PROCEDURAL APPROACH (C)                                           │
 *  ├──────────────────────────────────────────────────────────────────────┤
 *  │  - Data (struct) and functions are SEPARATE entities               │
 *  │  - Functions operate ON data that is passed as parameters          │
 *  │  - No encapsulation — any function can access any struct member    │
 *  │  - No data hiding — members are always accessible                 │
 *  │  - Code organized around ACTIONS (functions)                      │
 *  │                                                                    │
 *  │  Example in C:                                                     │
 *  │    typedef struct { char name[50]; float cgpa; } Student;          │
 *  │    char calculate_grade(float cgpa) { ... }                        │
 *  │    void print_report(Student *s) {                                 │
 *  │        printf("Name: %s\n", s->name);                              │
 *  │        printf("Grade: %c\n", calculate_grade(s->cgpa));            │
 *  │    }                                                               │
 *  │    // Data and operations are disconnected                         │
 *  └──────────────────────────────────────────────────────────────────────┘
 *
 *  ┌──────────────────────────────────────────────────────────────────────┐
 *  │  OBJECT-ORIENTED APPROACH (C++)                                    │
 *  ├──────────────────────────────────────────────────────────────────────┤
 *  │  - Data and functions are BUNDLED together inside classes          │
 *  │  - Data can be HIDDEN (private) — controlled via public methods   │
 *  │  - ENCAPSULATION protects data integrity                          │
 *  │  - Code organized around OBJECTS (data + behavior)                │
 *  │  - Supports inheritance, polymorphism, abstraction                │
 *  │                                                                    │
 *  │  Example in C++:                                                   │
 *  │    class ReportGenerator {                                         │
 *  │    private:                                                        │
 *  │        string name;    // Hidden — only class methods can access   │
 *  │        float cgpa;     // Protected from external modification     │
 *  │    public:                                                         │
 *  │        char getGrade() { ... }   // Behavior tied to data          │
 *  │        void printReport() { ... } // Object knows how to print     │
 *  │    };                                                              │
 *  │    // Data and operations are united — OOP principle               │
 *  └──────────────────────────────────────────────────────────────────────┘
 * ═══════════════════════════════════════════════════════════════════════════ */

/* ═══════════════════════════════════════════════════════════════════════════
 *  CLASS: ReportGenerator
 *
 *  Purpose      : Generates formatted student report cards using OOP.
 *  Private Data : studentName (string), cgpa (float)
 *  Public       : Constructor, printReport(), getGrade(), getReportCount()
 *  Static       : reportCount (shared across all objects)
 *  Covers       : Unit VI (C++ classes, static members, inline/non-inline)
 * ═══════════════════════════════════════════════════════════════════════════ */
class ReportGenerator {

private:
    /* ── Private data members ── */
    string studentName;     /* Accessible only within class methods */
    float  cgpa;            /* Protected from external modification */

public:
    /* ── Static data member: shared by ALL objects of this class ── */
    static int reportCount;

    /* ────────────────────────────────────────────────────────────────
     *  CONSTRUCTOR
     *
     *  Purpose : Initializes a ReportGenerator with student name & CGPA.
     *            Uses initializer list for efficient initialization.
     *  Params  : name — student's name (string)
     *            c    — student's CGPA (float)
     * ──────────────────────────────────────────────────────────────── */
    ReportGenerator(string name, float c) : studentName(name), cgpa(c)
    {
        reportCount++;  /* Shared counter — incremented for every object created */
        cout << "  [Constructor] ReportGenerator created for: " << studentName << endl;
    }

    /* ────────────────────────────────────────────────────────────────
     *  INLINE MEMBER FUNCTION: getGrade
     *
     *  Purpose : Returns the grade character based on CGPA.
     *            Defined INSIDE the class body — implicitly inline.
     *  Params  : None
     *  Returns : char — grade ('O','A','B','C','D','F')
     *
     *  Note: Inline functions are expanded at the call site by the
     *        compiler, avoiding function call overhead. Best for
     *        short, frequently-called functions.
     * ──────────────────────────────────────────────────────────────── */
    inline char getGrade()
    {
        if (cgpa >= 9.0f) return 'O';   /* Outstanding  */
        if (cgpa >= 8.0f) return 'A';
        if (cgpa >= 7.0f) return 'B';
        if (cgpa >= 6.0f) return 'C';
        if (cgpa >= 5.0f) return 'D';   /* Pass         */
        return 'F';                     /* Fail         */
    }

    /* ────────────────────────────────────────────────────────────────
     *  NON-INLINE MEMBER FUNCTION: printReport (declaration only)
     *
     *  Defined OUTSIDE the class body — this is a non-inline function.
     *  Non-inline functions are compiled as regular function calls.
     * ──────────────────────────────────────────────────────────────── */
    void printReport();

    /* ────────────────────────────────────────────────────────────────
     *  STATIC MEMBER FUNCTION: getReportCount
     *
     *  Purpose : Returns the total number of reports generated.
     *            Static member functions can be called without an
     *            object — using ClassName::functionName() syntax.
     *  Params  : None
     *  Returns : int — total report count
     *
     *  Note: Static functions can ONLY access static data members.
     *        They do NOT have a 'this' pointer.
     * ──────────────────────────────────────────────────────────────── */
    static int getReportCount()
    {
        return reportCount;
    }

    /* ── Getter for name (for external display if needed) ── */
    string getName() const { return studentName; }

    /* ── Getter for CGPA ── */
    float getCgpa() const { return cgpa; }
};

/* ═══════════════════════════════════════════════════════════════════════════
 *  STATIC MEMBER INITIALIZATION
 *
 *  Static data members must be defined and initialized OUTSIDE the class
 *  definition. This allocation happens at program startup, not when
 *  objects are created.
 * ═══════════════════════════════════════════════════════════════════════════ */
int ReportGenerator::reportCount = 0;

/* ═══════════════════════════════════════════════════════════════════════════
 *  NON-INLINE MEMBER FUNCTION DEFINITION: printReport
 *
 *  Purpose : Prints a complete, formatted report card to the console.
 *            Defined OUTSIDE the class (non-inline — not implicitly
 *            expanded at call site).
 *  Params  : None (uses 'this' pointer to access member data)
 *  Returns : void
 *  Covers  : Unit VI (non-inline member function, cout, string formatting)
 * ═══════════════════════════════════════════════════════════════════════════ */
void ReportGenerator::printReport()
{
    char grade = getGrade();        /* calls inline member function */
    string status = (cgpa >= 5.0f) ? "PASS" : "FAIL";
    string grade_text;

    switch (grade) {
        case 'O': grade_text = "Outstanding"; break;
        case 'A': grade_text = "Excellent";   break;
        case 'B': grade_text = "Good";        break;
        case 'C': grade_text = "Average";     break;
        case 'D': grade_text = "Below Avg";   break;
        case 'F': grade_text = "Fail";        break;
        default:  grade_text = "N/A";         break;
    }

    /* Formatted report card using cout (not printf) */
    cout << endl;
    cout << "  +============================================+" << endl;
    cout << "  |          STUDENT REPORT CARD                |" << endl;
    cout << "  |          (Generated by C++ Module)          |" << endl;
    cout << "  +============================================+" << endl;
    cout << "  |                                             |" << endl;
    cout << "  |  Name   : " << left << setw(32) << studentName << " |" << endl;
    cout << "  |  CGPA   : " << fixed << setprecision(2)
         << left << setw(32) << cgpa << " |" << endl;
    cout << "  |  Grade  : " << grade << " (" << left << setw(25) << grade_text << ")  |" << endl;
    cout << "  |  Status : " << left << setw(32) << status << " |" << endl;
    cout << "  |                                             |" << endl;

    /* Performance bar visualization */
    /*
     * Performance bar calculation:
     *   filled  = (int)(cgpa * 2)  — scales 0-10 CGPA to
     *             0-20 bar blocks (each block = 5% of score)
     *   display = cgpa * 10        — converts CGPA to 0-100
     *             score for human-readable output.
     * Example: CGPA 8.5 → 17 filled blocks → 85/100 score.
     */
    cout << "  |  Performance: [";
    int filled = (int)(cgpa * 2);   /* 20-char bar: cgpa*2 fills proportionally */
    for (int k = 0; k < 20; k++) {
        if (k < filled)
            cout << "#";
        else
            cout << "-";
    }
    cout << "] " << fixed << setprecision(1)
         << (cgpa * 10) << "/100" << "  |" << endl;
    cout << "  |  (Score = CGPA x 10, max CGPA is 10.0)          |"
         << endl;

    cout << "  |                                             |" << endl;
    cout << "  |  Report #" << left << setw(34) << reportCount << " |" << endl;
    cout << "  +============================================+" << endl;
}

/* ═══════════════════════════════════════════════════════════════════════════
 *  STRUCT IN C++ (for comparison with class)
 *
 *  This struct has the same capabilities as a class, but members are
 *  PUBLIC by default. Used here to contrast with ReportGenerator class.
 * ═══════════════════════════════════════════════════════════════════════════ */
struct StudentInfo {
    /* All members are PUBLIC by default (unlike class which is private) */
    string name;
    float  cgpa;

    /* Structs in C++ CAN have constructors and member functions */
    StudentInfo(string n, float c) : name(n), cgpa(c) {}

    void display() {
        cout << "  [Struct] " << name << " — CGPA: " << cgpa << endl;
    }
};

/* ═══════════════════════════════════════════════════════════════════════════
 *  MAIN FUNCTION — C++ Extension
 *
 *  Purpose : Demonstrates the ReportGenerator class with interactive
 *            input using cin/cout.
 *  Covers  : Unit VI (C++ basics — cin, cout, objects, class usage)
 * ═══════════════════════════════════════════════════════════════════════════ */
int main()
{
    cout << endl;
    cout << "  +========================================================+" << endl;
    cout << "  |                                                        |" << endl;
    cout << "  |      SRMS — C++ Extension Module                       |" << endl;
    cout << "  |      Unit VI: Basics of C++                            |" << endl;
    cout << "  |                                                        |" << endl;
    cout << "  +========================================================+" << endl;
    cout << endl;

    string name;
    float  cgpa;

    /* ── Object 1: Interactive Input ── */
    cout << "  ┌─── CREATE REPORT #1 ─────────────────────────────────┐" << endl;
    cout << "  │  Enter Student 1 Name: ";
    getline(cin, name);         /* cin — read full line (C++ I/O) */
    cout << "  │  Enter Student 1 CGPA (0-10): ";
    cin >> cgpa;                /* cin — read numeric value */
    cin.ignore();               /* discard trailing newline */
    cout << "  └──────────────────────────────────────────────────────┘" << endl;

    ReportGenerator report1(name, cgpa);    /* Object 1 created */
    report1.printReport();                  /* call non-inline method */

    /* ── Object 2: Interactive Input ── */
    cout << endl;
    cout << "  ┌─── CREATE REPORT #2 ─────────────────────────────────┐" << endl;
    cout << "  │  Enter Student 2 Name: ";
    getline(cin, name);
    cout << "  │  Enter Student 2 CGPA (0-10): ";
    cin >> cgpa;
    cin.ignore();
    cout << "  └──────────────────────────────────────────────────────┘" << endl;

    ReportGenerator report2(name, cgpa);    /* Object 2 created */
    report2.printReport();

    /* ── Static member function — called via class name (no object needed) ── */
    cout << endl;
    cout << "  ┌─── STATIC MEMBER DEMONSTRATION ──────────────────────┐" << endl;
    cout << "  │  ReportGenerator::getReportCount() = "
         << ReportGenerator::getReportCount() << endl;
    cout << "  │  (Static function called via ClassName:: — no object  │" << endl;
    cout << "  │   needed. Static data is shared across ALL objects.)  │" << endl;
    cout << "  └──────────────────────────────────────────────────────┘" << endl;

    /* ── Comparing inline vs non-inline ── */
    cout << endl;
    cout << "  ┌─── INLINE vs NON-INLINE COMPARISON ──────────────────┐" << endl;
    cout << "  │  report1.getGrade()    → '" << report1.getGrade() << "'"
         << "  [INLINE — defined inside class]      │" << endl;
    cout << "  │  report1.printReport() →     [NON-INLINE — defined outside class] │" << endl;
    cout << "  │                                                       │" << endl;
    cout << "  │  Inline: Compiler may expand code at call site        │" << endl;
    cout << "  │          (avoids function call overhead)               │" << endl;
    cout << "  │  Non-inline: Regular function call with stack frame   │" << endl;
    cout << "  └──────────────────────────────────────────────────────┘" << endl;

    /* ── Struct demonstration (comparing with class) ── */
    cout << endl;
    cout << "  ┌─── STRUCT vs CLASS IN C++ ───────────────────────────┐" << endl;
    StudentInfo info1("Demo Student", 7.5f);
    info1.display();
    cout << "  │  In C++, struct = class with PUBLIC default access.  │" << endl;
    cout << "  │  In C, struct = data-only container, no methods.     │" << endl;
    cout << "  └──────────────────────────────────────────────────────┘" << endl;

    /* ── Summary ── */
    cout << endl;
    cout << "  +========================================================+" << endl;
    cout << "  |  Total Reports Generated: " << setw(3)
         << ReportGenerator::getReportCount()
         << "                           |" << endl;
    cout << "  |  C++ Extension Module Complete.                        |" << endl;
    cout << "  +========================================================+" << endl;
    cout << endl;

    return 0;
}

/******************************************************************************
 *  END OF srms_report.cpp
 *
 *  Compilation: g++ -Wall -o srms_report srms_report.cpp
 *  Execution  : ./srms_report  or  srms_report.exe
 *
 *  KEY C++ CONCEPTS COVERED:
 *    ✓ Class with private/public access specifiers
 *    ✓ Constructor with initializer list
 *    ✓ Static data member (reportCount) — shared across objects
 *    ✓ Static member function (getReportCount) — callable via ClassName::
 *    ✓ Inline member function (getGrade) — defined inside class
 *    ✓ Non-inline member function (printReport) — defined outside class
 *    ✓ cin / cout (not scanf/printf)
 *    ✓ 2+ class objects created and methods called
 *    ✓ struct vs class difference explained
 *    ✓ Procedural (C) vs OOP (C++) comparison in comments
 ******************************************************************************/
