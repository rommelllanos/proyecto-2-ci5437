// Game of Othello -- Example of main
// Universidad Simon Bolivar, 2012.
// Author: Blai Bonet
// Last Revision: 1/11/16
// Modified by: Rommel Llanos 15-10789
//              Jesus Marcano 12-10359

#include <iostream>
#include <limits>
#include "othello_cut.h"
#include "utils.h"
#include <fstream>
#include <vector>
#include <stdbool.h>
#include <algorithm>
#include <string>
#include <time.h> 
#include <iomanip>
#include <unordered_map>

using namespace std;

unsigned expanded = 0;
unsigned generated = 0;
int tt_threshold = 32; // threshold to save entries in TT

// Transposition table (it is not necessary to implement TT)
struct stored_info_t {
    int value_;
    int type_;
    enum { EXACT, LOWER, UPPER };
    stored_info_t(int value = -100, int type = LOWER) : value_(value), type_(type) { }
};

struct hash_function_t {
    size_t operator()(const state_t &state) const {
        return state.hash();
    }
};

class hash_table_t : public unordered_map<state_t, stored_info_t, hash_function_t> {
};

hash_table_t TTable[2];

//int maxmin(state_t state, int depth, bool use_tt);
//int minmax(state_t state, int depth, bool use_tt = false);
//int maxmin(state_t state, int depth, bool use_tt = false);

vector<state_t> children(state_t state, int color) {
    // BorW(Black or White)
    // BorW is true for black and false for white
    bool BorW = (color == 1)? true:false;
    vector<state_t> moves;
    state_t new_state;

    // Keeps all the valids plays in moves
    for(int pos = 0; pos < DIM; ++pos) {
        if (state.outflank(BorW, pos)) {
            new_state = state.move(BorW, pos);
            moves.push_back(new_state);
        }
    }
    return moves;
};

int negamax(state_t state, int depth, int color, bool use_tt = false){
    ++generated;
    if (state.terminal()){
        return color * state.value();
    }
    // if not terminal state, then expand
    ++expanded;
    int alpha = numeric_limits<int>::min();
    // generating valids moves
    vector<state_t> child_states = children(state,color);
    
    if (child_states.size() == 0) {
        // No available moves (No modifications to the state), then the other color plays with the same state
        alpha = max(alpha, -negamax(state, ++depth, -color, use_tt));
    }

    else {
        for (state_t child : child_states) {
            alpha = max(alpha, -negamax(child, ++depth, -color, use_tt));
        }
    }
    return alpha;
};

// passing as args (alpha = MIN_INT, beta = MAX_INT) for initial call for max player 1
// negamax_ab(root_state, depth,-inf,inf,1 ) call for max player 
int negamax_ab(state_t state, int depth , int alpha, int beta, int color, bool use_tt = false){
    int score;
    int val;

    ++generated;
    if (state.terminal()){
        return color * state.value();
    }
    // if not terminal state, then expand
    ++expanded;
    score = numeric_limits<int>::min();
    // generating valids moves
    vector<state_t> child_states = children(state,color);
    
    if (child_states.size() == 0) {
        // No available moves (No modifications to the state), then the other color plays with the same state
        val = -negamax_ab(state, ++depth, -beta, -alpha, -color, use_tt);
        score = max(score,val);
    }

    else {
        for (state_t child : child_states) {
            val = -negamax_ab(child, ++depth, -beta, -alpha, -color, use_tt);
            score = max(score,val);
            alpha = max(alpha,val);
            if (alpha >= beta){
                break;
            } 
        }
    }
    return score;
};

// condition true for greater, false for greater equal
bool test(state_t node, int color, int depth, int score, bool condition){
    // The parameter "condition" is true if we want to use '>', and is false
    // if we want to use '>='
    if (node.terminal()) {
        if (condition) {
            return (node.value() > score) ? true : false;
        } else {
            return (node.value() >= score) ? true : false;
        }
    }

    vector<state_t> child_nodes = children(node, color);
    bool max_node = (color == 1) ? true : false;

    if (child_nodes.size() == 0) { 
        // No available moves (No modifications to the state), then the other color plays with the same state
        if (max_node && test(node, -color, depth++, score, condition)){
            return true;
        }

        if (!max_node && !test(node, -color, depth++, score, condition)){
            return false;
        }
    }

    for (state_t child : child_nodes) {
        if (max_node && test(child, -color, depth++, score, condition)){
            return true;
        }
        if (!max_node && !test(child, -color, depth++, score, condition)){
            return false;
        }
    }
    return max_node ? false : true;
}


int scout(state_t state, int depth, int color, bool use_tt = false) {
    ++generated;
    if (state.terminal()) {
        return state.value();
    }
    ++expanded;
    
    int score=0;
    int first_child = 1;
    bool max_node = (color == 1) ? true : false;
    vector<state_t> child_nodes = children(state, color);

    if (child_nodes.size() == 0) {
        // No available moves (No modifications to the state), then the other color plays with the same state
        score = scout(state, depth++, -color, use_tt);
    }
    
    for (state_t child : child_nodes) {
        if (first_child) {
            first_child = 0;
            score = scout(child, depth++, -color, use_tt);

        } 
        else {
            if (max_node && test(child, -color, depth++, score, true)){
                score = scout(child, depth++, -color, use_tt);
            }

            if (!max_node && !test(child, -color, depth++, score, false)){
                score = scout(child, depth++, -color, use_tt);
            }
        }
    }
    return score;
}

int negascout(state_t state, int depth, int alpha, int beta, int color, bool use_tt = false){
    int score;
    int firstChild = 1;

    ++generated;
    if (state.terminal()){
        return color * state.value();
    }

    // if not terminal state, then expand
    ++expanded;
    // generating valids moves
    vector<state_t> childrens = children(state,color);

    if (childrens.size() == 0) {
        // No available moves (No modifications to the state), then the other color plays with the same state
        score = -negascout(state, ++depth,-beta,-alpha,-color);
        alpha = max(alpha, score);
    }

    else{
        for (state_t child : childrens) {
            if (firstChild){
                firstChild = 0;
                score = -negascout(child, ++depth, -beta, -alpha, -color);
            }
            else{
                score = -negascout(child, ++depth, -alpha - 1, -alpha, -color);
                if ((alpha < score) && (score < beta)){
                    score = -negascout(child, ++depth,-beta,-score,-color);
                }
            }
            alpha = std::max(alpha,score);
            if (alpha >= beta){
                break;
            }
        }
    }
    return alpha;
};

int main(int argc, const char **argv) {
    state_t pv[128];
    int npv = 0;
    for( int i = 0; PV[i] != -1; ++i ) ++npv;

    int algorithm = 0;
    if( argc > 1 ) algorithm = atoi(argv[1]);
    bool use_tt = argc > 2;

    // Extract principal variation of the game
    state_t state;
    cout << "Extracting principal variation (PV) with " << npv << " plays ... " << flush;
    for( int i = 0; PV[i] != -1; ++i ) {
        bool player = i % 2 == 0; // black moves first!
        int pos = PV[i];
        pv[npv - i] = state;
        state = state.move(player, pos);
    }
    pv[0] = state;
    cout << "done!" << endl;

#if 0
    //print principal variation
    for( int i = 0; i <= npv; ++i )
        cout << pv[npv - i];
#endif

    // Print name of algorithm
    cout << "Algorithm: ";
    if( algorithm == 1 )
        cout << "Negamax (minmax version)";
    else if( algorithm == 2 )
        cout << "Negamax (alpha-beta version)";
    else if( algorithm == 3 )
        cout << "Scout";
    else if( algorithm == 4 )
        cout << "Negascout";
    cout << (use_tt ? " w/ transposition table" : "") << endl;

    // Run algorithm along PV (bacwards)
    cout << "Moving along PV:" << endl;
    for( int i = 0; i <= npv; ++i ) {
        //cout << pv[i];
        int value = 0;
        TTable[0].clear();
        TTable[1].clear();
        float start_time = Utils::read_time_in_seconds();
        expanded = 0;
        generated = 0;
        int color = i % 2 == 1 ? 1 : -1;

        try {
            if( algorithm == 1 ) {
                value = negamax(pv[i], 0, color, use_tt);
            } else if( algorithm == 2 ) {
                value = negamax_ab(pv[i], 0, -200, 200, color, use_tt);
            } else if( algorithm == 3 ) {
                value = color * scout(pv[i], 0, color, use_tt);
            } else if( algorithm == 4 ) {
                value = negascout(pv[i], 0, -200, 200, color, use_tt);
            }
        } catch( const bad_alloc &e ) {
            cout << "size TT[0]: size=" << TTable[0].size() << ", #buckets=" << TTable[0].bucket_count() << endl;
            cout << "size TT[1]: size=" << TTable[1].size() << ", #buckets=" << TTable[1].bucket_count() << endl;
            use_tt = false;
        }

        float elapsed_time = Utils::read_time_in_seconds() - start_time;

        cout << npv + 1 - i << ". " << (color == 1 ? "Black" : "White") << " moves: "
             << "value=" << color * value
             << ", #expanded=" << expanded
             << ", #generated=" << generated
             << ", seconds=" << elapsed_time
             << ", #generated/second=" << generated/elapsed_time
             << endl;
    }
    return 0;
}


