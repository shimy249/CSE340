extern "C"
{
#include "lexer.h"
}

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

class Rule;
class RuleList;
class Grammar;

class Rule{
public:
    string term;
    bool nonTerm;

    Rule(string ele, bool nonT){
        term = ele;
        nonTerm = nonT;
    }
};

class RuleList{
public:
    vector<Rule*> elements;

    RuleList(vector<Rule*> data){
        elements = data;
    }

};


class Grammar{
public:
    string lhs;
    vector<RuleList*> rhs;
    bool epsilon;
    Grammar(string l){
        lhs = l;
        epsilon = false;
    }


};

int debug = 0;

//check if element is a lhs
bool isNonTerm(string ele, vector<Grammar*>rules){
    vector<Grammar*>::iterator it;
    for(it = rules.begin(); it != rules.end(); ++it){
        if((*it)->lhs==ele)//matches lhs
            return true;
    }
    return false;
}

bool isEpsilon(vector<Grammar*> master, string lhs){
    for(vector<Grammar*>::iterator git = master.begin(); git != master.end(); ++git){
        if((*git)->lhs==lhs && (*git)->epsilon == true){
            return true;
        }
    }
    return false;
}

//++++++++++Get First Set++++++++++
vector<string> first(vector<Grammar*> master, string lhs){
    vector<string> res;

    for(vector<Grammar*>::iterator git = master.begin(); git != master.end(); ++git){
        if((*git)->lhs == lhs){ //found Grammar
            if((*git)->epsilon)
                res.insert(res.begin(), "#");
            for(vector<RuleList*>::iterator rlit = (*git)->rhs.begin(); rlit != (*git)->rhs.end(); ++rlit){
                if((*rlit)->elements.front()->nonTerm == false){//if first is terminal
                    if (find(res.begin(), res.end(), (*rlit)->elements.front()->term) == res.end()) {//add only if unique
                        res.push_back((*rlit)->elements.front()->term);
                    }
                }
                if((*rlit)->elements.front()->nonTerm==true){//if first is nonterminal

                    for(vector<Rule*>::iterator rit = (*rlit)->elements.begin(); rit != (*rlit)->elements.end(); ++rit){//iterate till non term
                        if((*rit)->nonTerm==false){ //terminal
                            if (find(res.begin(), res.end(), (*rit)->term) == res.end()) {//add only if unique
                                res.push_back((*rit)->term);
                            }
                        }
                        else{
                            if((*rit) ==(*) )
                        }
                    }


                }
            }
        }

    }

    return res;
}

int main (int argc, char* argv[])
{
    int task;

    if (argc < 2)
    {
        cout << "Error: missing argument" << endl;
        return 1;
    }

    /*
       Note that by convention argv[0] is the name of your executable,
       and the first argument to your program is stored in argv[1]
     */

    task = atoi(argv[1]);

    /*
       Hint: You can modify and use the lexer from previous project
       to read the input. Note that there are only 4 token types needed
       for reading the input in this project.

       WARNING: You will need to modify lexer.c and lexer.h to support
                the project 3 input language.
     */

    vector <Grammar*> rules;

    //+++++++++Get Non Terminals++++++++++++

    token_type type = getToken();

    while(type != HASH){

        Grammar *temp = new Grammar(current_token);
        rules.push_back(temp);
        type = getToken();
    }

    //OK

    //++++++++Begin rule parse++++++++++++
    type = getToken();
    while(type !=DOUBLEHASH) {

        string lhs = current_token; //lhs

        type = getToken();//should be arrow

        if (type == ARROW) {//just in case

            type = getToken();//should be term/nonterm or hash
            vector<Rule*> currRhs;//rhs list for current lhs
            debug = 0;
            while (type != HASH) {
                debug++;
                currRhs.push_back(new Rule(current_token, isNonTerm(current_token, rules)));

                type = getToken();//next item on rhs
            }//end of rhs list now add to lhs

            if(debug == 0){

                currRhs.push_back(new Rule("#", false));
            }
            vector<Grammar*>::iterator it;
            for(it = rules.begin(); it != rules.end(); ++it){
                if((*it)->lhs==lhs){

                    (*it)->rhs.push_back(new RuleList(currRhs));
                    if(currRhs.front()->term == "#")
                        (*it)->epsilon = true;

                }
            }//inserted into rulelist


        }
        type = getToken();
    }

    switch (task) {
        case 0: {

            vector<string> terminals;

            //begin iteration through rule lists
            for (vector<Grammar*>::iterator git = rules.begin(); git != rules.end(); ++git) {//iterate LHS

                for (vector<RuleList*>::iterator rlit = (*git)->rhs.begin(); rlit != (*git)->rhs.end(); ++rlit) {//iterate different rule lists

                    for (vector<Rule *>::iterator rit = (*rlit)->elements.begin(); rit != (*rlit)->elements.end(); ++rit) {//iterate through rule list

                        if ((*rit)->nonTerm == false) {
                            if (find(terminals.begin(), terminals.end(), (*rit)->term) == terminals.end()) {//add only if unique
                                terminals.push_back((*rit)->term);
                            }
                        }
                    }

                }

            }//done iteration


            //+++++print terminals++++++++
            vector<string>::iterator it;
            for (it = terminals.begin(); it != terminals.end(); ++it) {

                cout << *it << " ";

            }

            cout << endl; //line break for terminal list

            //+++++print lhs counts+++++
            //OK
            for (vector<Grammar*>::iterator git = rules.begin(); git != rules.end(); ++git) {

                cout << (*git)->lhs << ": " << (*git)->rhs.size() << endl;

            }

            break;
        }
        case 1: {
            for (vector<Grammar *>::iterator git = rules.begin(); git != rules.end(); ++git) {
                //cout << "Calling" <<endl;
                vector<string> firstset = first(rules, (*git)->lhs);
                //cout << "done calling" << endl;
                cout << "FIRST(" << (*git)->lhs << ") = { ";

                for (vector<string>::iterator fit = firstset.begin(); fit != firstset.end() - 1; ++fit) {
                    cout << *fit << ", ";
                }
                cout << firstset.back() << " }" << endl;

            }

            break;
        }
        case 2:
            // TODO: Calculate FIRST sets for the input grammar
            // TODO: Calculate FOLLOW sets for the input grammar
            // TODO: Output the FOLLOW sets in the exact order and format required
            break;

        default:
            cout << "Error: unrecognized task number " << task << endl;
            break;
    }
    return 0;
}