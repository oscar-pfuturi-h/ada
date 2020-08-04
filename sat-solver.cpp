#include <algorithm>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <string>
#include <vector>
#include <stdio.h>

using namespace std;

enum Cat {
  satisfied,
  unsatisfied,
  normal,
  completed
};


class Formula {
public:
  vector<vector<int> > literals;
  vector<int> literal_frequency;
  vector<int> literal_polarity;
  vector<vector<int> > clauses;
  Formula() {}

  Formula(const Formula &f) {
    literals = f.literals;
    clauses = f.clauses;
    literal_frequency = f.literal_frequency;
    literal_polarity = f.literal_polarity;
  }
};

class SATSolver {
private:
  Formula formula;               // formula inicial en cnf
  int literal_count;             // numero de variables en la formula
  int clause_count;              // numero de clausulas en la formula
  int unit_propagate(Formula &); // performs unit propagation
  int DPLL(Formula);             // performs DPLL recursively
  int apply_transform(Formula &, int); // applies the value of the literal in every clause
  void show_result(Formula &); // displays the result
  int get_values(Formula &);
  int is_satisfiable(Formula &, int);
public:
  SATSolver() {}
  void initialize(); // initializes the values
  void solve();      // calls the solver
};

void SATSolver::initialize() {
  char c;
  string s;

  while (true) {
    cin >> c;
    if (c == 'c'){
      getline(cin, s);
    }
    else{
      cin >> s;
      break;
    }
  }
  cin >> literal_count;
  cin >> clause_count;

  formula.literals.clear();
  formula.literals.resize( pow(2,literal_count) );

  int razon;
	int fi;
	int cont;
	int i, div = 2;

	fi = pow(2,literal_count);

	for(int j=0; j<literal_count; j++){
		i = 0;
		cont = 1;
		razon = fi/div;
		do{
			for(int fil=i; fil < i+razon; fil++){
				if(cont%2!=0){
					formula.literals[fil].push_back(-1);
				}
				else{
					formula.literals[fil].push_back(1);
				}
			}
			cont++;
			i += razon;
		}while (i<fi);

		div *= 2;
	}

  /*for(int fil=0; fil < fi; fil++){
    for(int col=0; col<literal_count; col++){
        int res = formula.literals[fil][col];
        cout<< res <<" ";
    }
    cout<<endl;
  }*/

  formula.clauses.clear();
  formula.clauses.resize(clause_count);
  formula.literal_frequency.clear();
  formula.literal_frequency.resize(literal_count, 0);
  formula.literal_polarity.clear();
  formula.literal_polarity.resize(literal_count, 0);

  int literal; // store the incoming literal value

  for (int i = 0; i < clause_count; i++) {
    while (true) // while the ith clause gets more literals
    {
      cin >> literal;
      if (literal != 0){
      	formula.clauses[i].push_back(literal);
      }
	  else {
        break; // read 0, so move to next clause
      }
    }
  }
}

int SATSolver::get_values(Formula & f){
	int k = 0;
	int s;
	while (k < f.literals.size()){
		s = is_satisfiable(f,k);
		if (s == Cat::unsatisfied){
			f.literals.erase(f.literals.begin()+k);
			k--;
		}
		k++;
	}
	if (f.literals.size() > 0){
		return Cat::satisfied;
	} else {
		return Cat::unsatisfied;
	}
}

int SATSolver::is_satisfiable(Formula & f, int k){
	int s;
	for (int i=0; i<clause_count; i++){
		for (int j=0; j<f.clauses[i].size(); j++){
			if(f.literals[k][abs(f.clauses[i][j])-1] * (f.clauses[i][j]) > 0){
				s = Cat::satisfied;
				break;//la clausula es verdadera
			}
			else {
				s = Cat::unsatisfied;
			}
		}
		if (s == Cat::unsatisfied){//el conjunto de valores no satisfacen el problema
			return Cat::unsatisfied;
		}
	}
	return Cat::satisfied;
}

void SATSolver::show_result(Formula & f) {
    cout << "Es satisfacible para:" << endl;
    int l;
    for (int i = 0; i < f.literals.size(); i++) {
    	for (int j = 0; j < f.literals[i].size(); j++){
        l = *(f.literals[i].begin()+j) * (j+1);
    		cout<< l <<" ";
    	}
    	cout<<endl;
    }
}

void SATSolver::solve() {
  int result = get_values(formula);
  if (result == Cat::satisfied){
  	show_result(formula);
  } else {
  	cout<<endl<<"No es satisfacible"<<endl;
  }
}

int main() {
  SATSolver solver; // create the solver
  solver.initialize();  // initialize
  solver.solve();       // solve
  return 0;
}
