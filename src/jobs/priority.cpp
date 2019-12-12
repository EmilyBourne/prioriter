#include <priority.hpp>

int compare(Priority p1, Priority p2)
{
    // Possible combinations : LM, LH, ML, MH, HL, HM
    if (p1 == LOW) return 1; // LM, LH
    else if (p2 == LOW) return -1; // ML, HL
    else if (p1 == MEDIUM) return 1; // MH
    else return -1; //HM
}
