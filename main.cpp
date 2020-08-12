#include <algorithm>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <string>
#include <vector>
#include <bits/stdc++.h>
#include <stdlib.h>

using namespace std;

enum Cat {
  satisfied,
  unsatisfied
};


class Formula {
public:
  vector<vector<int> > literals;
  vector<int> literal_polarity;
  vector<vector<int> > clauses;
  Formula() {}

  Formula(const Formula &f) {
    literals = f.literals;
    clauses = f.clauses;
    literal_polarity = f.literal_polarity;
  }
};

class SATSolver {
private:
  Formula formula;               // formula inicial en cnf
  int literal_count;             // numero de variables en la formula
  int clause_count;              // numero de clausulas en la formula
  void show_result(Formula &);   // imprime los resultados
  int get_values(Formula &);
  int is_satisfiable(Formula &, int);
  void get_tabletf(int);
public:
  SATSolver() {}
  void initialize(string);
  void solve();
  Formula get_formula() {return formula;}

  SATSolver operator * (SATSolver b){
      SATSolver s;
      int cont = 0;
      int flag;
      for (unsigned int i=0; i<formula.literals.size(); i++){
        for (unsigned int j=0; j<b.formula.literals.size(); j++){
            for (int k=0; k<literal_count; k++){
                if (formula.literals[i][k] == b.formula.literals[j][k]){
                    flag = satisfied;
                } else { flag = unsatisfied; break;}
            }
            if (flag == satisfied){
                cont++;
                s.formula.literals.resize(cont);
                for (int p=0; p<literal_count; p++){
                    s.formula.literals[cont-1].push_back(formula.literals[i][p]);
                }
            }
        }
      }
      return s;
  }
  SATSolver operator - (SATSolver b){
      SATSolver s;
      int cont = 0;
      int flag;
      for (unsigned int i=0; i<formula.literals.size(); i++){
        for (unsigned int j=0; j<b.formula.literals.size(); j++){
            for (int k=0; k<literal_count; k++){
                if (formula.literals[i][k] == b.formula.literals[j][k]){
                    flag = 0;
                } else { flag = 1; break;}
            }
            if (flag == 0){
                break;
            }
        }
        if (flag == 1){
            cont++;
            s.formula.literals.resize(cont);
            for (int p=0; p<literal_count; p++){
                s.formula.literals[cont-1].push_back(formula.literals[i][p]);
            }
        }
      }
      return s;
  }
};

void SATSolver::get_tabletf(int n){
    int razon;
	int fi;
	int cont;
	int i, div = 2;

	fi = pow(2,n);

	for(int j=0; j<n; j++){
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
	/*for(int fil=0; fil < fi; fil++){ imprime la tabla
    for(int col=0; col<literal_count; col++){
        int res = formula.literals[fil][col];
        cout<< res <<" ";
    }
    cout<<endl;
    }*/
}

void SATSolver::initialize(string f_name) {
    fstream reader;
    reader.open(f_name, fstream::in);
    char c;
    string s;

  while (true) {
    reader >> c;
    //cin >> c;
    if (c == 'c'){
      getline(reader, s);
    }
    else{
      reader >> s;
  //cin >> s
      break;
    }
  }
  reader >> literal_count;
  reader >> clause_count;
  //cin >> literal_count;
  //cin >> clause_count;

  formula.literals.clear();
  formula.literals.resize( pow(2,literal_count) );

  get_tabletf(literal_count);

  formula.clauses.clear();
  formula.clauses.resize(clause_count);
  formula.literal_polarity.clear();
  formula.literal_polarity.resize(literal_count, 0);

  int literal;

  for (int i = 0; i < clause_count; i++) {
    while (true)
    {
        reader >> literal;
      //cin >> literal;
      if (literal != 0){
      	formula.clauses[i].push_back(literal);
      }
	  else {
        break; // lee 0, asi que se mueve a la siguiente clausula
      }
    }
  }
}

int SATSolver::get_values(Formula & f){
	unsigned int k = 0;
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
		for (unsigned int j=0; j<f.clauses[i].size(); j++){
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
    for (unsigned int i = 0; i < f.literals.size(); i++) {
    	for (unsigned int j = 0; j < f.literals[i].size(); j++){
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
    //crear un solver para un archivo de entrada
    SATSolver solver;
    solver.initialize("input1.txt");
    solver.solve();

    //crear un segundo solver
    SATSolver solver2;
    solver2.initialize("input2.txt");
    solver2.solve();

    SATSolver solver3;
    solver3 = solver * solver2;
    int val;
    cout<<"\nSatisfacible para:\n";
    for (unsigned int i=0; i<solver3.get_formula().literals.size(); i++){
        for (unsigned int j=0; j<solver3.get_formula().literals[i].size(); j++){
            val = *(solver3.get_formula().literals[i].begin()+j) * (j+1);
            cout<< val << " ";
        }
        cout<<endl;
    }

    return 0;
}
