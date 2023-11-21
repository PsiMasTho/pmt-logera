// Generated by Flexc++ V2.14.00 on Thu, 16 Nov 2023 23:21:54 +0100

#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <filesystem>

// $insert class_ih
#include "lexer.h"


    // s_ranges_: use (unsigned) characters as index to obtain
    //           that character's range-number.
    //           The range for EOF is defined in a constant in the
    //           class header file
size_t const lexer_base::s_ranges_[] =
{
     0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 3, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
     5, 5, 5, 5, 5, 5, 5, 6, 7, 7, 8, 9, 9, 9, 9, 9, 9, 9, 9, 9,10,11,12,13,13,
    13,13,13,13,13,13,13,13,14,15,16,16,17,18,18,19,19,19,19,19,19,19,19,19,19,
    19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,20,20,20,20,21,22,23,24,24,
    24,24,24,24,24,24,24,24,24,24,24,24,24,24,25,26,27,28,29,30,30,30,30,31,31,
    31,31,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,
    32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,
    32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,
    32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,
    32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,
    32,32,32,32,32,32,
};

// $insert startcondinfo
    // s_dfa_ contains the rows of *all* DFAs ordered by start state.  The
    // enum class StartCondition_is defined in the baseclass header.
    // StartCondition_::INITIAL is always 0.  Each entry defines the row to
    // transit to if the column's character range was sensed. Row numbers are
    // relative to the used DFA, and d_dfaBase_ is set to the first row of
    // the subset to use.  The row's final two values are respectively the
    // rule that may be matched at this state, and the rule's FINAL flag. If
    // the final value equals FINAL (= 1) then, if there's no continuation,
    // the rule is matched. If the BOL flag (8) is also set (so FINAL + BOL (=
    // 9) is set) then the rule only matches when d_atBOL is also true.
int const lexer_base::s_dfa_[][36] =
{
    // INITIAL
    { 1, 2, 3, 2, 4, 1, 2, 1, 5, 1, 1, 1, 1, 6, 1, 1, 1, 7, 1, 1,
          1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,-1,    -1, -1},  // 0
    {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
         -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,    48, -1},  // 1
    {-1, 2,-1, 2,-1,-1, 2,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
         -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,    47, -1},  // 2
    {-1,-1, 3,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
         -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,    46, -1},  // 3
    {-1,-1, 3,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
         -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,    48, -1},  // 4
    { 5, 5, 8, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
          5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,-1,    44, -1},  // 5
    {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1, 9,-1,-1,-1,-1,-1,-1,
         -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,    48, -1},  // 6
    {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
         -1,-1,-1,10,-1,-1,-1,-1,-1,11,-1,-1,-1,-1,    48, -1},  // 7
    {-1,-1, 8,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
         -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,    45, -1},  // 8
    {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,12,-1,-1,-1,-1,-1,-1,
         -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,    -1, -1},  // 9
    {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
         -1,-1,-1,-1,-1,-1,-1,13,-1,-1,-1,-1,-1,-1,    -1, -1},  // 10
    {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
         -1,-1,-1,14,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,    -1, -1},  // 11
    {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,15,-1,-1,-1,-1,-1,-1,
         -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,    -1, -1},  // 12
    {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
         -1,-1,-1,-1,-1,-1,-1,16,-1,-1,-1,-1,-1,-1,    -1, -1},  // 13
    {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
         -1,-1,-1,-1,-1,17,-1,-1,-1,-1,-1,-1,-1,-1,    -1, -1},  // 14
    {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,18,-1,-1,-1,-1,-1,-1,-1,-1,-1,
         -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,    -1, -1},  // 15
    {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
         -1,-1,-1,-1,-1,19,-1,-1,-1,-1,-1,-1,-1,-1,    -1, -1},  // 16
    {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
         -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,    14, -1},  // 17
    {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,20,-1,-1,-1,-1,-1,-1,
         -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,    -1, -1},  // 18
    {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
         -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,     0, -1},  // 19
    {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,21,-1,-1,-1,-1,-1,-1,
         -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,    -1, -1},  // 20
    {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,22,-1,-1,-1,-1,-1,-1,-1,-1,-1,
         -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,    -1, -1},  // 21
    {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,23,-1,-1,-1,-1,-1,-1,
         -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,    -1, -1},  // 22
    {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,24,-1,-1,-1,-1,-1,-1,
         -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,    -1, -1},  // 23
    {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
         -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,    28, -1},  // 24
    // decl_attr_initial
    { 1, 2, 3, 2, 4, 1, 2, 1, 5, 1, 1, 1, 1, 1, 1, 6, 1, 1, 1, 7,
          1, 7, 1, 7, 7, 7, 7, 7, 7, 7, 7, 1, 1,-1,    -1, -1},  // 0
    {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
         -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,     7, -1},  // 1
    {-1, 2,-1, 2,-1,-1, 2,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
         -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,     6, -1},  // 2
    {-1,-1, 3,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
         -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,     3, -1},  // 3
    {-1,-1, 3,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
         -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,     7, -1},  // 4
    { 5, 5, 8, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
          5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,-1,     1, -1},  // 5
    {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
         -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,     5, -1},  // 6
    {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1, 9,-1, 9,-1,-1,-1,-1,-1, 7,
         -1, 7,-1, 7, 7, 7, 7, 7, 7, 7, 7,-1,-1,-1,     4, -1},  // 7
    {-1,-1, 8,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
         -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,     2, -1},  // 8
    {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1, 9,-1, 9,-1,-1,-1,-1,-1, 9,
         -1, 9,-1, 9, 9, 9, 9, 9, 9, 9, 9,-1,-1,-1,     4, -1},  // 9
    // decl_attr_regex_seq
    { 1, 2, 3, 2, 4, 1, 2, 5, 6, 5, 5, 5, 5, 5, 5, 1, 5, 5, 5, 5,
          5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 1,-1,    -1, -1},  // 0
    {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
         -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,    13, -1},  // 1
    {-1, 2,-1, 2,-1,-1, 2,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
         -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,    12, -1},  // 2
    {-1,-1, 3,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
         -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,    10, -1},  // 3
    {-1,-1, 3,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
         -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,    13, -1},  // 4
    {-1,-1,-1,-1,-1,-1,-1, 5,-1, 5, 5, 5, 5, 5, 5,-1, 5, 5, 5, 5,
          5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,-1,-1,    11, -1},  // 5
    { 6, 6, 7, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
          6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,-1,     8, -1},  // 6
    {-1,-1, 7,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
         -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,     9, -1},  // 7
    // decl_var_initial
    { 1, 2, 3, 2, 4, 1, 2, 1, 5, 1, 1, 1, 1, 1, 1, 6, 1, 1, 1, 7,
          1, 7, 1, 7, 7, 7, 7, 7, 7, 7, 7, 1, 1,-1,    -1, -1},  // 0
    {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
         -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,    21, -1},  // 1
    {-1, 2,-1, 2,-1,-1, 2,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
         -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,    20, -1},  // 2
    {-1,-1, 3,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
         -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,    17, -1},  // 3
    {-1,-1, 3,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
         -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,    21, -1},  // 4
    { 5, 5, 8, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
          5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,-1,    15, -1},  // 5
    {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
         -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,    19, -1},  // 6
    {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1, 9,-1, 9,-1,-1,-1,-1,-1, 7,
         -1, 7,-1, 7, 7, 7, 7, 7, 7, 7, 7,-1,-1,-1,    18, -1},  // 7
    {-1,-1, 8,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
         -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,    16, -1},  // 8
    {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1, 9,-1, 9,-1,-1,-1,-1,-1, 9,
         -1, 9,-1, 9, 9, 9, 9, 9, 9, 9, 9,-1,-1,-1,    18, -1},  // 9
    // decl_var_value_seq
    { 1, 2, 3, 2, 4, 1, 2, 1, 5, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 6,
          1, 6, 1, 6, 6, 6, 6, 6, 6, 6, 6, 1, 1,-1,    -1, -1},  // 0
    {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
         -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,    27, -1},  // 1
    {-1, 2,-1, 2,-1,-1, 2,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
         -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,    26, -1},  // 2
    {-1,-1, 3,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
         -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,    24, -1},  // 3
    {-1,-1, 3,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
         -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,    27, -1},  // 4
    { 5, 5, 7, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
          5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,-1,    22, -1},  // 5
    {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1, 8,-1, 8,-1,-1,-1,-1,-1, 6,
         -1, 6,-1, 6, 6, 6, 6, 6, 6, 6, 6,-1,-1,-1,    25, -1},  // 6
    {-1,-1, 7,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
         -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,    23, -1},  // 7
    {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1, 8,-1, 8,-1,-1,-1,-1,-1, 8,
         -1, 8,-1, 8, 8, 8, 8, 8, 8, 8, 8,-1,-1,-1,    25, -1},  // 8
    // logfile_initial
    { 1, 2, 3, 2, 4, 1, 2, 1, 5, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 6,
          1, 6, 1, 6, 6, 6, 6, 6, 6, 6, 6, 1, 1,-1,    -1, -1},  // 0
    {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
         -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,    34, -1},  // 1
    {-1, 2,-1, 2,-1,-1, 2,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
         -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,    33, -1},  // 2
    {-1,-1, 3,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
         -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,    31, -1},  // 3
    {-1,-1, 3,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
         -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,    34, -1},  // 4
    { 5, 5, 7, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
          5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,-1,    29, -1},  // 5
    {-1, 8, 8, 8, 8,-1, 8,-1,-1,-1,-1, 9,-1, 9,10,-1,-1,-1,-1,11,
         -1,11,-1,11,11,11,11,11,11,11,11,-1,-1,-1,    34, -1},  // 6
    {-1,-1, 7,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
         -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,    30, -1},  // 7
    {-1, 8, 8, 8, 8,-1, 8,-1,-1,-1,-1,-1,-1,-1,10,-1,-1,-1,-1,-1,
         -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,    -1, -1},  // 8
    {-1, 8, 8, 8, 8,-1, 8,-1,-1,-1,-1, 9,-1, 9,10,-1,-1,-1,-1, 9,
         -1, 9,-1, 9, 9, 9, 9, 9, 9, 9, 9,-1,-1,-1,    -1, -1},  // 9
    {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
         -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,    32, -1},  // 10
    {-1, 8, 8, 8, 8,-1, 8,-1,-1,-1,-1, 9,-1, 9,10,-1,-1,-1,-1,11,
         -1,11,-1,11,11,11,11,11,11,11,11,-1,-1,-1,    -1, -1},  // 11
    // logfile_attr_val_seq
    { 1, 2, 3, 2, 4, 1, 2, 5, 6, 5, 5, 5, 5, 5, 7, 1, 5, 5, 5, 8,
          5, 8, 5, 8, 8, 8, 8, 8, 8, 8, 8, 5, 1,-1,    -1, -1},  // 0
    {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
         -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,    43, -1},  // 1
    {-1, 2,-1, 2,-1,-1, 2,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
         -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,    42, -1},  // 2
    {-1,-1, 3,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
         -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,    37, -1},  // 3
    {-1,-1, 3,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
         -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,    43, -1},  // 4
    {-1,-1,-1,-1,-1,-1,-1, 5,-1, 5, 5, 5, 5, 5, 5,-1, 5, 5, 5, 5,
          5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,-1,-1,    41, -1},  // 5
    { 6, 6, 9, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
          6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,-1,    35, -1},  // 6
    {-1,-1,-1,-1,-1,-1,-1, 5,-1, 5, 5, 5, 5, 5, 5,-1, 5, 5, 5, 5,
          5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,-1,-1,    38, -1},  // 7
    {-1,10,11,10,11,-1,10, 5,-1, 5, 5,12, 5,12,13,-1, 5, 5, 5, 8,
          5, 8, 5, 8, 8, 8, 8, 8, 8, 8, 8, 5,-1,-1,    41, -1},  // 8
    {-1,-1, 9,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
         -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,    36, -1},  // 9
    {-1,10,11,10,11,-1,10,14,-1,14,14,14,14,14,15,-1,14,14,14,14,
         14,14,14,14,14,14,14,14,14,14,14,14,-1,-1,    -1, -1},  // 10
    {-1,11,11,11,11,-1,11,-1,-1,-1,-1,-1,-1,-1,16,-1,-1,-1,-1,-1,
         -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,    -1, -1},  // 11
    {-1,10,11,10,11,-1,10, 5,-1, 5, 5,12, 5,12,13,-1, 5, 5, 5,12,
          5,12, 5,12,12,12,12,12,12,12,12, 5,-1,-1,    41, -1},  // 12
    {-1,-1,-1,-1,-1,-1,-1, 5,-1, 5, 5, 5, 5, 5, 5,-1, 5, 5, 5, 5,
          5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,-1,-1,    39, -1},  // 13
    {-1,-1,-1,-1,-1,-1,-1,14,-1,14,14,14,14,14,14,-1,14,14,14,14,
         14,14,14,14,14,14,14,14,14,14,14,14,-1,-1,    40, -1},  // 14
    {-1,-1,-1,-1,-1,-1,-1,14,-1,14,14,14,14,14,14,-1,14,14,14,14,
         14,14,14,14,14,14,14,14,14,14,14,14,-1,-1,    39, -1},  // 15
    {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
         -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,    39, -1},  // 16
    //  7
    { 1, 2, 2, 2, 2, 1, 2, 1, 1, 1, 1, 1, 1, 1, 3, 1, 1, 1, 1, 1,
          1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,-1,    -1, -1},  // 0
    {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
         -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,    53, -1},  // 1
    {-1, 4, 4, 4, 4,-1, 4,-1,-1,-1,-1,-1,-1,-1, 3,-1,-1,-1,-1,-1,
         -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,    53, -1},  // 2
    {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
         -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,    49, -1},  // 3
    {-1, 4, 4, 4, 4,-1, 4,-1,-1,-1,-1,-1,-1,-1, 3,-1,-1,-1,-1,-1,
         -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,    -1, -1},  // 4
    //  8
    {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1, 1,
         -1, 1,-1, 1, 1, 1, 1, 1, 1, 1, 1,-1,-1,-1,    -1, -1},  // 0
    {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1, 2,-1, 2,-1,-1,-1,-1,-1, 1,
         -1, 1,-1, 1, 1, 1, 1, 1, 1, 1, 1,-1,-1,-1,    50, -1},  // 1
    {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1, 2,-1, 2,-1,-1,-1,-1,-1, 2,
         -1, 2,-1, 2, 2, 2, 2, 2, 2, 2, 2,-1,-1,-1,    50, -1},  // 2
    //  9
    { 1, 2, 2, 2, 2, 1, 2, 1, 1, 1, 1, 1, 1, 1, 3, 1, 1, 1, 1, 1,
          1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,-1,    -1, -1},  // 0
    {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
         -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,    53, -1},  // 1
    {-1, 4, 4, 4, 4,-1, 4,-1,-1,-1,-1,-1,-1,-1, 3,-1,-1,-1,-1,-1,
         -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,    53, -1},  // 2
    {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
         -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,    51, -1},  // 3
    {-1, 4, 4, 4, 4,-1, 4,-1,-1,-1,-1,-1,-1,-1, 3,-1,-1,-1,-1,-1,
         -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,    -1, -1},  // 4
    //  10
    {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1, 1,
         -1, 1,-1, 1, 1, 1, 1, 1, 1, 1, 1,-1,-1,-1,    -1, -1},  // 0
    {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1, 2,-1, 2,-1,-1,-1,-1,-1, 1,
         -1, 1,-1, 1, 1, 1, 1, 1, 1, 1, 1,-1,-1,-1,    52, -1},  // 1
    {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1, 2,-1, 2,-1,-1,-1,-1,-1, 2,
         -1, 2,-1, 2, 2, 2, 2, 2, 2, 2, 2,-1,-1,-1,    52, -1},  // 2
};


int const (*lexer_base::s_dfaBase_[])[36] =
{
    s_dfa_ + 0,
    s_dfa_ + 25,
    s_dfa_ + 35,
    s_dfa_ + 43,
    s_dfa_ + 53,
    s_dfa_ + 62,
    s_dfa_ + 74,
    s_dfa_ + 91,
    s_dfa_ + 96,
    s_dfa_ + 99,
    s_dfa_ + 104,
};

size_t lexer_base::s_istreamNr = 0;

// $insert inputImplementation
lexer_base::Input::Input()
:
    d_in(0),
    d_lineNr(1)
{}

lexer_base::Input::Input(std::istream *iStream, size_t lineNr)
:
    d_in(iStream),
    d_lineNr(lineNr)
{}

size_t lexer_base::Input::get()
{
    switch (size_t ch = next())         // get the next input char
    {
        case '\n':
            ++d_lineNr;
        [[fallthrough]];

        default:
        return ch;
    }
}

size_t lexer_base::Input::next()
{
    size_t ch;

    if (d_deque.empty())                // deque empty: next char fm d_in
    {
        if (d_in == 0)
            return AT_EOF;
        ch = d_in->get();
        return *d_in ? ch : static_cast<size_t>(AT_EOF);
    }

    ch = d_deque.front();
    d_deque.pop_front();

    return ch;
}

void lexer_base::Input::reRead(size_t ch)
{
    if (ch < 0x100)
    {
        if (ch == '\n')
            --d_lineNr;
        d_deque.push_front(ch);
    }
}

void lexer_base::Input::reRead(std::string const &str, size_t fm)
{
    for (size_t idx = str.size(); idx-- > fm; )
        reRead(str[idx]);
}

lexer_base::lexer_base(std::istream &in, std::ostream &out, bool keepCwd)
:
    d_out(new std::ostream(out.rdbuf())),
// $insert interactiveInit
    d_in(0),
    d_dfaBase_(s_dfa_)
{
    if (keepCwd)
        d_cwd = std::filesystem::current_path().string();
    p_pushStream(s_istream, new std::istream(in.rdbuf()));
}

void lexer_base::switchStream_(std::istream &in, size_t lineNr)
{
    d_input->close();
    d_streamStack.back().input = { new std::istream(in.rdbuf()), lineNr };
}


lexer_base::lexer_base(std::string const &infilename, std::string const &outfilename, bool keepCwd)
:
    d_out(outfilename == "-"    ? new std::ostream(std::cout.rdbuf()) :
          outfilename == ""     ? new std::ostream(std::cerr.rdbuf()) :
                                  new std::ofstream(outfilename)),
    d_dfaBase_(s_dfa_)
{
    if (keepCwd)
        d_cwd = std::filesystem::current_path().string();

    p_pushStream(infilename, { new std::ifstream(infilename) });
    toCwd();
}

void lexer_base::switchStreams(std::istream &in, std::ostream &out)
{
    switchStream_(in, 1);
    switchOstream(out);
}

void lexer_base::switchIstream(std::string const &infilename)
{
    d_input->close();
    d_filename = infilename;

    d_streamStack.back() = 
        StreamStruct{ chgWorkingDir(d_filename), 
                      { new std::ifstream(infilename) } };

    d_atBOL = true;
}

void lexer_base::switchStreams(std::string const &infilename,
                           std::string const &outfilename)
{
    switchOstream(outfilename);
    switchIstream(infilename);
}

void lexer_base::pushStream(std::istream  &istr)
{
    p_pushStream("(istream)", new std::istream(istr.rdbuf())); //streamPtr);
}

void lexer_base::pushStream(std::string const &name)
{
    std::istream *streamPtr = new std::ifstream(name);
    if (!*streamPtr)
    {
        delete streamPtr;
        throw std::runtime_error("Cannot read " + name);
    }
    p_pushStream(name, streamPtr);
}



void lexer_base::toCwd() const
{
    /*
    using namespace std;
    using namespace filesystem;

    if (d_cwd.empty())
        return;

    error_code ec;
    current_path(d_cwd, ec);
    */
};

// static
std::string lexer_base::chgWorkingDir(std::string const &name)     // any name
{
    using namespace std;
    using namespace filesystem;

    if (name == s_istream)                              // used with istreams
        return name;
                                                        // a filename: get its
    auto path = absolute(name);                         // full pathname

    error_code ec;
    current_path(path.parent_path(), ec);

    return path.string();                               // return the pathname
}

void lexer_base::p_pushStream(std::string const &name, std::istream *streamPtr)
{
    if (d_streamStack.size() == s_maxSizeofStreamStack_)
    {
        delete streamPtr;
        throw std::length_error("Max stream stack size exceeded");
    }

    d_filename = name;
    d_streamStack.push_back(
                    StreamStruct{chgWorkingDir(d_filename), { streamPtr } }
                  );

    d_input = &d_streamStack.back().input;

    d_atBOL = true;
}

void lexer_base::switchOstream(std::ostream &out)
{
    *d_out << std::flush;
    d_out.reset(new std::ostream(out.rdbuf()));
}

// $insert debugFunctions
void lexer_base::setDebug(bool onOff)
{}

bool lexer_base::debug() const
{
    return false;
}

void lexer_base::redo(size_t nChars)
{
    size_t from = nChars >= length() ? 0 : length() - nChars;
    d_input->reRead(m_matched, from);
    m_matched.resize(from);
}

void lexer_base::switchOstream(std::string const &outfilename)
{
    *d_out << std::flush;
    d_out.reset(
            outfilename == "-"    ? new std::ostream(std::cout.rdbuf()) :
            outfilename == ""     ? new std::ostream(std::cerr.rdbuf()) :
                                    new std::ofstream(outfilename));
}


bool lexer_base::popStream()
{
    using namespace std::filesystem;

    d_input->close();

    if (d_streamStack.size() <= 1)
        return false;

    d_streamStack.pop_back();

    d_filename = path{ chgWorkingDir(d_streamStack.back().pushedName) 
                     }.filename().string();

    d_input = &d_streamStack.back().input;

    return true;
}

// $insert lopImplementation
void lexer_base::lop1_(int lopSC)
{
    d_lopMatched = m_matched;
    d_lopPending = d_input->nPending();

    d_lopEnd = d_lopMatched.end();
    d_lopTail = d_lopEnd - 1;
    d_lopIter = d_lopTail;

    d_get = &lexer_base::getLOP;

    d_lopSC = d_startCondition;             // remember original SC
    begin(SC(lopSC));                       // activate the 
                                            // tail-matching SC
}
void lexer_base::lop2_()   // matched the tail
{
    d_lopEnd = d_lopTail;                   // read the head
    d_lopIter = d_lopMatched.begin();

    begin(SC(d_startCondition + 1));        // switch to the head-matching
}                                           // SC
inline void lexer_base::lop3_() // catch-all handler
{
    d_lopIter = --d_lopTail;                // increase the tail, try again
}
void lexer_base::lop4_()
{

    begin(SC(d_lopSC));                     // restore original SC
    d_get = &lexer_base::getInput;          // restore get function

    d_input->setPending(d_lopPending);
                                            // reinsert the tail into the 
                                            // input stream
    push(d_lopMatched.substr(length(), std::string::npos));
}
size_t lexer_base::getLOP()
{
    size_t ch = d_lopIter == d_lopEnd ? as<size_t>(AT_EOF) : *d_lopIter++;

    return ch;
}


  // See the manual's section `Run-time operations' section for an explanation
  // of this member.
lexer_base::ActionType_ lexer_base::actionType_(size_t range)
{
    d_nextState = d_dfaBase_[d_state][range];

    if (d_nextState != -1)                  // transition is possible
        return ActionType_::CONTINUE;

    if (knownFinalState())                  // FINAL state reached
        return ActionType_::MATCH;         

    if (m_matched.size())
        return ActionType_::ECHO_FIRST;    // no match, echo the 1st char

    return range != s_rangeOfEOF_ ? 
                ActionType_::ECHO_CH 
            : 
                ActionType_::RETURN;
}

void lexer_base::accept(size_t nChars)          // old name: less
{
    if (nChars < m_matched.size())
    {
        d_input->reRead(m_matched, nChars);
        m_matched.resize(nChars);
    }
}

void lexer_base::setMatchedSize(size_t length)
{
    d_input->reRead(m_matched, length);  // reread the tail section
    m_matched.resize(length);           // return what's left
}

  // At this point a rule has been matched.  The next character is not part of
  // the matched rule and is sent back to the input.  The final match length
  // is determined, the index of the matched rule is determined, and then
  // d_atBOL is updated. Finally the rule's index is returned.
  // The numbers behind the finalPtr assignments are explained in the 
  // manual's `Run-time operations' section.
size_t lexer_base::matched_(size_t ch)
{
    d_input->reRead(ch);

    FinalData *finalPtr;
                            
    if (not d_atBOL)                    // not at BOL
        finalPtr = &d_final.std;        // then use the std rule (3, 4)

                                        // at BOL
    else if (not available(d_final.std.rule))   // only a BOL rule avail.
            finalPtr = &d_final.bol;            // use the BOL rule (6)

    else if (not available(d_final.bol.rule)) // only a std rule is avail.
        finalPtr = &d_final.std;        // use the std rule (7)
        
    else if (                           // Both are available (8)
        d_final.bol.length !=           // check lengths of matched texts
        d_final.std.length              // unequal lengths, use the rule
    )                                   // having the longer match length
        finalPtr =              
            d_final.bol.length > d_final.std.length ?
                &d_final.bol
            :
                &d_final.std;

    else                            // lengths are equal: use 1st rule
        finalPtr = 
            d_final.bol.rule < d_final.std.rule ?
                &d_final.bol
            :
                &d_final.std;

    setMatchedSize(finalPtr->length);

    d_atBOL = m_matched.back() == '\n';


    return finalPtr->rule;
}

size_t lexer_base::getRange_(int ch)       // using int to prevent casts
{
    return ch == AT_EOF ? as<size_t>(s_rangeOfEOF_) : s_ranges_[ch];
}

  // At this point d_nextState contains the next state and continuation is
  // possible. The just read char. is appended to d_match
void lexer_base::continue_(int ch)
{
    d_state = d_nextState;

    if (ch != AT_EOF)
        m_matched += ch;
}

void lexer_base::echoCh_(size_t ch)
{
    *d_out << as<char>(ch);
    d_atBOL = ch == '\n';
}


   // At this point there is no continuation. The last character is
   // pushed back into the input stream as well as all but the first char. in
   // the buffer. The first char. in the buffer is echoed to stderr. 
   // If there isn't any 1st char yet then the current char doesn't fit any
   // rules and that char is then echoed
void lexer_base::echoFirst_(size_t ch)
{
    d_input->reRead(ch);
    d_input->reRead(m_matched, 1);
    echoCh_(m_matched[0]);
}

    // Update the rules associated with the current state, do this separately
    // for BOL and std rules.
    // If a rule was set, update the rule index and the current m_matched
    // length. 
void lexer_base::updateFinals_()
{
    size_t len = m_matched.size();

    int const *rf = d_dfaBase_[d_state] + s_finIdx_;

    if (rf[0] != -1)        // update to the latest std rule
    {
        d_final.std = FinalData { as<size_t>(rf[0]), len };
    }

    if (rf[1] != -1)        // update to the latest bol rule
    {
        d_final.bol = FinalData { as<size_t>(rf[1]), len };
    }
}

void lexer_base::reset_()
{
    d_final = Final{ 
                    FinalData{s_unavailable, 0}, 
                    FinalData {s_unavailable, 0} 
                };

    d_state = 0;
    d_return = true;

    if (!d_more)
        m_matched.clear();

    d_more = false;
}

int lexer::executeAction_(size_t ruleIdx)
try
{
    switch (ruleIdx)
    {
        // $insert actions
        case 0:
        {
            {
                begin(StartCondition_::decl_attr_initial);
                return tokens::Tokens_::DECL_ATTR;
            }
        }
        break;
        case 2:
        case 3:
        {
            {
                begin(StartCondition_::INITIAL);
                return tokens::Tokens_::NEWLINE;
            }
        }
        break;
        case 4:
        {
            return tokens::Tokens_::IDENT;
        }
        break;
        case 5:
        {
            {
                begin(StartCondition_::decl_attr_regex_seq);
                return ';';
            }
        }
        break;
        case 7:
        {
            return matched()[0];
        }
        break;
        case 9:
        case 10:
        {
            {
                begin(StartCondition_::INITIAL);
                return tokens::Tokens_::NEWLINE;
            }
        }
        break;
        case 11:
        {
            return tokens::Tokens_::REGEX;
        }
        break;
        case 13:
        {
            return matched()[0];
        }
        break;
        case 14:
        {
            {
                begin(StartCondition_::decl_var_initial);
                return tokens::Tokens_::DECL_VAR;
            }
        }
        break;
        case 16:
        case 17:
        {
            {
                begin(StartCondition_::INITIAL);
                return tokens::Tokens_::NEWLINE;
            }
        }
        break;
        case 18:
        {
            return tokens::Tokens_::IDENT;
        }
        break;
        case 19:
        {
            {
                begin(StartCondition_::decl_var_value_seq);
                return ';';
            }
        }
        break;
        case 21:
        {
            return matched()[0];
        }
        break;
        case 23:
        case 24:
        {
            {
                begin(StartCondition_::INITIAL);
                return tokens::Tokens_::NEWLINE;
            }
        }
        break;
        case 25:
        {
            return tokens::Tokens_::IDENT;
        }
        break;
        case 27:
        {
            return matched()[0];
        }
        break;
        case 28:
        {
            {
                begin(StartCondition_::logfile_initial);
                return tokens::Tokens_::DATE;
            }
        }
        break;
        case 30:
        case 31:
        {
            return tokens::Tokens_::NEWLINE;
        }
        break;
        case 34:
        {
            return matched()[0];
        }
        break;
        case 36:
        case 37:
        {
            return tokens::Tokens_::NEWLINE;
        }
        break;
        case 38:
        {
            return matched()[0];
        }
        break;
        case 40:
        {
            {
                
                std::size_t const split = strchr(matched().data(), ' ') - matched().data();
                accept(split);
                return tokens::Tokens_::IDENT;
            }
        }
        break;
        case 41:
        {
            return tokens::Tokens_::ATTR_VALUE;
        }
        break;
        case 43:
        {
            return matched()[0];
        }
        break;
        case 45:
        case 46:
        {
            return tokens::Tokens_::NEWLINE;
        }
        break;
        case 48:
        {
            return matched()[0];
        }
        break;
        case 32:
        {
            lop1_(7);
        }
        break;
        case 39:
        {
            lop1_(9);
        }
        break;
        case 50:
        {
            lop4_();
            {
                
                begin(StartCondition_::logfile_attr_val_seq);
                return tokens::Tokens_::IDENT;
            }
        }
        break;
        case 52:
        {
            lop4_();
            {
                accept(0);
                begin(StartCondition_::logfile_initial);
            }
        }
        break;
        case 51:
        case 49:
        {
            lop2_();
        }
        break;
        case 53:
        {
            lop3_();
        }
        break;
    }
    noReturn_();
    return 0;
}
catch (Leave_ value)
{
    return static_cast<int>(value);
}

int lexer::lex_()
{
    reset_();
    preCode();

    while (true)
    {
        size_t ch = get_();                // fetch next char
        size_t range = getRange_(ch);      // determine the range

        updateFinals_();                    // update the state's Final info

        switch (actionType_(range))        // determine the action
        {
            case ActionType_::CONTINUE:
                continue_(ch);
            continue;

            case ActionType_::MATCH:
            {
                d_token_ = executeAction_(matched_(ch));
                if (return_())
                {
                    print();
                    toCwd();
                    return d_token_;
                }
                break;
            }

            case ActionType_::ECHO_FIRST:
                echoFirst_(ch);
            break;

            case ActionType_::ECHO_CH:
                echoCh_(ch);
            break;

            case ActionType_::RETURN:
                if (!popStream())
                {
                     postCode(PostEnum_::END);
                     toCwd();
                     return 0;
                }
                postCode(PostEnum_::POP);
             continue;
        } // switch

        postCode(PostEnum_::WIP);

        reset_();
        preCode();
    } // while
}

void lexer_base::print_() const
{
}


