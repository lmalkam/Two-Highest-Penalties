#include<bits/stdc++.h>
using namespace std;

void printMatrix(vector<vector<float>>&matrix){
	for(int i=0;i<matrix.size();i++){
		for(int j=0;j<matrix[i].size();j++){
			cout<<matrix[i][j]<<" ";
		}
		cout<<endl;
	}
}

void THP(vector<vector<float>>&matrix, vector<float>&supplies,vector<float>&demands){
	int num_supplies = supplies.size(), num_demands = demands.size();
	vector<vector<float>> allocation_matrix(num_supplies,vector<float>(num_demands,0));
	vector<bool>is_row_eliminated(num_supplies,0);
	vector<bool>is_col_elimintaed(num_demands,0);
	//should perform till all rows and cols are eliminated
	int not_terminated = num_supplies + num_demands;
	int iterations = 1;
	float total_cost = 0;
	while(not_terminated){
		cout<<"ITERATION - "<<iterations++<<endl<<endl;
	//	Step 2. Determine the Penalty for each row and column
		vector<float>row_penalities(num_supplies,-1);
		vector<float>col_penalities(num_demands,-1);
		vector<int>row_min_costs(num_supplies);
		vector<int>col_min_costs(num_demands);
		//row penalities
		cout<<"Row penalities : ";
		for(int i=0;i<num_supplies;i++){
			//find Max Cost
			if(!is_row_eliminated[i]){//calculate penality for rows that are not eliminated
				float maxCost = INT_MIN;
				float minCost = INT_MAX;
				int count = 0;
				for(int j=0;j<num_demands;j++){ //consider only those cells whose col is not eliminated
					if(!is_col_elimintaed[j]){
						maxCost = max(maxCost,matrix[i][j]);
						if(minCost > matrix[i][j]){
							minCost = matrix[i][j];
							row_min_costs[i] = j;
						}	
						count++;
					}
				}
				if(count>1){
					row_penalities[i] = maxCost - minCost;
					cout<<row_penalities[i]<<" ";	
				}else{
					cout<<"-- ";
				}
					
			}else{
				cout<<"NA ";
			}
		}
		cout<<endl;
		
		//col penalities
		cout<<"Column penalities : ";
		for(int j=0;j<num_demands;j++){
			//find Max Cost
			if(!is_col_elimintaed[j]){//calculate penality for cols that are not eliminated
				float maxCost = INT_MIN;
				float minCost = INT_MAX;
				int count = 0;
				for(int i=0;i<num_supplies;i++){
					//consider only those cells whose row is not eliminated
					if(!is_row_eliminated[i]){
						maxCost = max(maxCost,matrix[i][j]);
						if(minCost > matrix[i][j]){
							minCost = matrix[i][j];
							col_min_costs[j] = i;
						}	
						count++;
					}
				}
				if(count>1){
					col_penalities[j] = maxCost - minCost;
					cout<<col_penalities[j]<<" ";
				}else{
					cout<<"-- ";
				}	
			}else{
				cout<<"NA ";
			}
			
		}
		cout<<endl<<endl;
		
	//	Step 3. Select the Penalty and Cell
		map<float,vector<pair<int,int>>>mp;
		//mp[penality] = {is it a row/col penality,at what index};
		for(int i=0;i<row_penalities.size();i++){
			if(row_penalities[i] != -1)
			mp[row_penalities[i]].push_back({0,i});
		}
		for(int j=0;j<col_penalities.size();j++){
			if(col_penalities[j] != -1)
			mp[col_penalities[j]].push_back({1,j});
		}
		
		if(mp.size() == 0){
			//If only one element is left
			int row = -1;
			int col = -1;
			for(int i=0;i<is_row_eliminated.size();i++){
				if(!is_row_eliminated[i]){
					row = i;
					break;
				}
			}
			for(int j=0;j<is_col_elimintaed.size();j++){
				if(!is_col_elimintaed[j]){
					col = j;
					break;
				}
			}
			if(row == -1 || col == -1){
				cout<<"ERROR!!\n";
			}else{
				allocation_matrix[row][col] = min(supplies[row],demands[col]);
				total_cost += matrix[row][col]*allocation_matrix[row][col];
				cout<<"Allocated only left cell ("<<row + 1<<","<<col + 1<<") with "<< allocation_matrix[row][col] <<" units \n\n";
				
				cout<<"Allocation Matrix : \n";
				printMatrix(allocation_matrix);
				
				supplies[row] -= allocation_matrix[row][col];
				demands[col] -= allocation_matrix[row][col];
				if(supplies[row] == 0){
					is_row_eliminated[row] = 1;
					not_terminated--;
				}
				if(demands[col] == 0){
					is_col_elimintaed[col] = 1;
					not_terminated--;
				}
			}
			break;
		}
		auto it = mp.rbegin();
		
		float highest_penality = it->first;
		float next_highest_penality ;
		vector<pair<int,int>> indices = it->second;
		
		pair<int,int> selected_cell = {-1,-1};
		float selected_cell_CA = INT_MAX;
		
		cout<<"Highest Penality at : \n";
		//to avoid code duplication when computing for HP and next HP
		for(int i=0;i<2;i++){//iterating over choosen penalities
			for(auto ele : indices){
				int isCol = ele.first;
				int index = ele.second;
				float min_cost, allocation , CA;
				int row,col;
				if(!isCol){
					cout<<"S"<<index + 1<<" -> ";
					row = index;
					col = row_min_costs[index];
					min_cost = matrix[row][col];
				}else{
					cout<<"D"<<index + 1<<" -> ";
					row = col_min_costs[index];
					col = index;
					min_cost = matrix[row][col];
				}
				allocation = min(supplies[row],demands[col]);
				CA = min_cost * allocation;
				//selecting cell
				if(CA < selected_cell_CA){
					selected_cell_CA = CA;
					selected_cell = {row,col};
				}else if(CA == selected_cell_CA){
					if(min_cost < matrix[selected_cell.first][selected_cell.second]){
						selected_cell_CA = CA;
						selected_cell = {row,col};
					}
				}
				cout<<"CA = "<<min_cost<<"*"<<allocation<<" = "<<CA<<" at cell ("<<row + 1<<","<<col + 1<<")\n";
			}
			
			//moving to next highest penality
			if(i == 0){
				it++;
				if(it == mp.rend()){
					break;
				}
				next_highest_penality = it->first;
				indices.clear();
				indices = it->second;
				cout<<"Next Highest Penality at : \n";
			}	
		}
		
	//	Step 4. Allocate the Cell 
		int selected_cell_row = selected_cell.first, selected_cell_col = selected_cell.second;
		float allocation = selected_cell_CA/matrix[selected_cell_row][selected_cell_col];
		cout<<"Selected Cell : ("<<selected_cell_row + 1<<","<<selected_cell_col + 1<<") with allocation of "<<allocation<<" units\n\n";
		
		//allocate cell
		allocation_matrix[selected_cell_row][selected_cell_col] = allocation;
		total_cost += matrix[selected_cell_row][selected_cell_col]*allocation;
		
		cout<<"Allocation Matrix : \n";
		printMatrix(allocation_matrix);
		cout<<endl;
		
		//update supplies and demands
		supplies[selected_cell_row] -= allocation;
		demands[selected_cell_col] -= allocation;
		
	//	Step 5. Eliminate Row or Column 
		if(supplies[selected_cell_row] == 0){
			is_row_eliminated[selected_cell_row] = 1;
			not_terminated--;
		}
		if(demands[selected_cell_col] == 0){
			is_col_elimintaed[selected_cell_col] = 1;
			not_terminated--;
		}	
	}
	
	cout<<"Total Cost = "<<total_cost<<endl;

}

int main(){
	
	vector<float> supplies;
	vector<float> demands;
	float input;
	float total_supply = 0,total_demand = 0;
	
	//input supplies
	cout<<"Enter the Supplies : ";
	cin>>input;
	while(input != -1){
		supplies.push_back(input);
		total_supply += input;
		cin>>input;
	}
	
	//input demands
	cout<<"Enter the Demands : ";
	cin>>input;
	while(input != -1){
		demands.push_back(input);
		total_demand += input;
		cin>>input;
	}
	
	vector<vector<float>>matrix(supplies.size(),vector<float>(demands.size()));
	//input cost matrix
	cout<<"Enter cost matrix : \n";
	for(int i=0;i<matrix.size();i++){
		for(int j=0;j<matrix[i].size();j++){
			cin>>matrix[i][j];
		}
	}
	
//	Step 1. Preliminaries
	//check if given instance is balanced or not
	cout<<"Total Supply : "<<total_supply<<endl;
	cout<<"Total demand : "<<total_demand<<endl;
	if(total_demand > total_supply){
		supplies.push_back(total_demand - total_supply);
		vector<float>dummy_row(demands.size(),0);
		matrix.push_back(dummy_row);
	}else if(total_supply > total_demand){
		demands.push_back(total_supply - total_demand);
		vector<float>dummy_col(supplies.size(),0);
		for(int i=0;i<matrix.size();i++){
			matrix[i].push_back(dummy_col[i]);
		}
	}else{
		cout<<"Given instance is already balanced\n\n";
	}
	
	//print the matrix
	THP(matrix,supplies,demands);
	
}

/*
50 55 75 60 -1
90 65 30 55 -1
6 3 1 4
7 6 2 1
10 4 5 9
7 7 7 3

150 175 275 -1
200 100 300 -1
6 8 10
7 11 11
4 5 12

18 17 19 13 15 -1
16 18 20 14 14 -1
70 37 6 76 17
59 90 93 5 10
93 62 77 47 62
54 55 26 9 84
53 20 84 15 9
*/
