#include<iostream>
#include<utility>
#include<vector>
#include<map>
#include<bits/stdc++.h>
using namespace std;
void print(float tm[10][10],int m,int n){
	for(int i=0;i<m;i++)
	{
		for(int j=0;j<n;j++){
			cout<<tm[i][j]<<" ";
		}
		cout<<endl;
	}
}
void northwest(int m,int n,float supply_row[],float demand_col[],pair<pair<int,int>,float> solution[]){
	int count=0,i=0,j=0;
	while(count<(m+n)){
		if(supply_row[i]>=demand_col[j]){
			supply_row[i]-=demand_col[j];
			solution[count].first.first=i;
			solution[count].first.second=j;
			solution[count].second=demand_col[j];
			demand_col[j]=0;
			j++;
			count++;
		}else{
			demand_col[j]-=supply_row[i];
			solution[count].first.first=i;
			solution[count].first.second=j;
			solution[count].second=supply_row[i];
			supply_row[i]=0;
			i++;
			count++;
		}
	}
}
bool isthere(vector<int> v,int key){
	for(auto i:v){
		if(i==key)
		return true;
	}
	return false;
}
pair<int,int> findmin(float tm[10][10],int m,int n,vector<int> i_striked,vector<int> j_striked){
	float min=INT_MAX;
	pair<int,int> p;
	for(int i=0;i<m;i++){
		for(int j=0;j<n;j++){
			if(tm[i][j]<min)
			if(!isthere(i_striked,i) && !isthere(j_striked,j)){
				p.first=i;p.second=j;
				min=tm[i][j];
			}
		}
	}
	return p;
}
void least_cost(float tm[10][10],int m,int n,float supply_row[],float demand_col[],pair<pair<int,int>,float> solution[],int counts,vector<int> i_striked,vector<int> j_striked){
	int count=counts;
	while(count<(m+n)){
		pair<int,int> p=findmin(tm,m,n,i_striked,j_striked);
		int i=p.first,j=p.second;
		solution[count].first.first=i;
		solution[count].first.second=j;
		if(supply_row[i]>=demand_col[j]){
			supply_row[i]-=demand_col[j];
			solution[count].second=demand_col[j];
			demand_col[j]=0;
			j_striked.push_back(j);
		}else{
			demand_col[j]-=supply_row[i];
			solution[count].second=supply_row[i];
			supply_row[i]=0;
			i_striked.push_back(i);
		}
		count++;
	}
}
float penality(float arr[],int n){
	float min=INT_MAX;
	int index=-1;
	for(int i=0;i<n;i++){
		if(min>arr[i]){
			min=arr[i];
			index=i;
		}
	}
	float sec_min=INT_MAX;
	for(int i=0;i<n;i++){
		if(sec_min>arr[i] && i!=index){
			sec_min=arr[i];
		}
	}
	return sec_min-min;
}
vector<int> findmax(float arr[],int n){
	float max=-1;
	vector<int>index;
	for(int i=0;i<n;i++){
		if(max<arr[i]){
			max=arr[i];
		}
	}
	for(int i=0;i<n;i++){
		if(arr[i]==max)
		index.push_back(i);
	}
	return index;
}
int findmin(float arr[],int n){
	float min=INT_MAX;
	int index=-1;
	for(int i=0;i<n;i++){
		if(min>arr[i]){
			min=arr[i];
			index=i;
		}
	}
	return index;
}
int min(float arr[],int n){
	float min=INT_MAX;
	int index=-1;
	for(int i=0;i<n;i++){
		if(min>arr[i]){
			min=arr[i];
			index=i;
		}
	}
	return min;
}
void cal_penalities(float tm[10][10],float row_penality[],float col_penality[],int m,int n,float supply_row[],float demand_col[]){
	for(int i=0;i<m;i++){
		if(supply_row[i]!=-1 ){
			float arr[n];
			for(int j=0;j<n;j++){
				if(demand_col[j]==-1){
					arr[j]=INT_MAX;
				}else arr[j]=tm[i][j];
			}
			row_penality[i]=penality(arr,n);
		}
		else row_penality[i]=-1;
	}
	for(int j=0;j<n;j++){
		if(demand_col[j]!=-1){
			float arr[m];
			for(int i=0;i<m;i++){
				if(supply_row[i]==-1){
					arr[i]=INT_MAX;
				}else arr[i]=tm[i][j];
			}
			col_penality[j]=penality(arr,m);
		}
		else col_penality[j]=-1;
	}
	
}
bool solve(float row_penality[],float col_penality[],int m,int n){
	int count=0;
	for(int i=0;i<m;i++){
		if(row_penality[i]!=-1)count++;
	}
	if(count<2) 
	return false;
	count=0;
	for(int i=0;i<n;i++){
		if(col_penality[i]!=-1)count++;
	}
	if(count<2) 
	return false;
	return true;
}
void vam(float tm[10][10],int m,int n,pair<pair<int,int>,float> solution[],float supply_row[],float demand_col[]){
	int count=0;
	vector<int> i_striked,j_striked;
	while(1){
		float row_penality[m],col_penality[n];
		cal_penalities(tm,row_penality,col_penality,m,n,supply_row,demand_col);
		if(!solve(row_penality,col_penality,m,n)){
			for(int i=0;i<m;i++){
				if(row_penality[i]==-1){
					i_striked.push_back(i);
				}
			}
			for(int i=0;i<n;i++){
				if(col_penality[i]==-1){
					j_striked.push_back(i);
				}
			}	
			break;
		}
		vector<int>row_i=findmax(row_penality,m);
		vector<int>col_j=findmax(col_penality,n);
		int sol_i,sol_j;
		float val[10];
		float cost[10];
		if(row_penality[row_i[0]]>col_penality[col_j[0]]){
			for(int i=0;i<row_i.size();i++){
				for(int j=0;j<n;j++){
					cost[j]=tm[row_i[i]][j];
				}
				val[i]=min(cost,n);
			}
			sol_i=row_i[findmin(val,row_i.size())];
			float arr[n];
			for(int j=0;j<n;j++){
				if(demand_col[j]!=-1)
				arr[j]=tm[sol_i][j];
				else arr[j]=INT_MAX;
			}
			sol_j=findmin(arr,n);
			cout<<"allocated cell: "<<"i="<<sol_i<<" j="<<sol_j<<endl;
		}else{
			for(int i=0;i<col_j.size();i++){
				for(int j=0;j<m;j++){
					cost[j]=tm[j][col_j[i]];
				}
				val[i]=min(cost,m);
			}
			sol_j=col_j[findmin(val,col_j.size())];
			float arr[m];
			for(int i=0;i<m;i++){
				if(supply_row[i]!=-1)
				arr[i]=tm[i][sol_j];
				else arr[i]=INT_MAX;
			}
			sol_i=findmin(arr,m);
			cout<<"allocated cell: "<<"i="<<sol_i<<" j="<<sol_j<<endl;
		}
		solution[count].first.first=sol_i;
		solution[count].first.second=sol_j;
		if(supply_row[sol_i]>=demand_col[sol_j]){
			supply_row[sol_i]-=demand_col[sol_j];
			solution[count].second=demand_col[sol_j];
			demand_col[sol_j]=-1;
		}else{
			demand_col[sol_j]-=supply_row[sol_i];
			solution[count].second=supply_row[sol_i];
			supply_row[sol_i]=-1;
		}
		count++;	
	}
	least_cost(tm,m,n,supply_row,demand_col,solution,count,i_striked,j_striked);
}
int main(){
	int m,n;
	cin>>m>>n;
	float tm[10][10];
	for(int i=0;i<m;i++)
	for(int j=0;j<n;j++)
	cin>>tm[i][j];
	float supply_row[m+1],demand_col[n+1];
	float supply=0,demand=0;
	for(int i=0;i<m;i++){
		cin>>supply_row[i];
		supply+=supply_row[i];
	}
	for(int i=0;i<n;i++){
		cin>>demand_col[i];
		demand+=demand_col[i];
	}
	if(supply>demand){
		for(int i=0;i<m;i++)
		tm[i][n]=0;
		demand_col[n]=supply-demand;
		n++;
	}
	else if(demand>supply){
		for(int j=0;j<n;j++)
		tm[m][j]=0;
		supply_row[m]=demand-supply;
		m++;
	}
	pair<pair<int,int>,float> solution[m+n-1];
	//northwest(m,n,supply_row,demand_col,solution);
	/*for(int i=0;i<(m+n-1);i++){
		cout<<solution[i].first.first<<" "<<solution[i].first.second<<" "<<solution[i].second<<endl;
	}*/
	/*least_cost(tm,m,n,supply_row,demand_col,solution);
	for(int i=0;i<(m+n-1);i++)
	cout<<solution[i].first.first<<" "<<solution[i].first.second<<" "<<solution[i].second<<endl;
*/
	vam(tm,m,n,solution,supply_row,demand_col);
	float total_cost = 0;
	cout<<"Allocation cells - allocation :\n";
	for(int i=0;i<(m+n-1);i++){
		cout<<solution[i].first.first<<" "<<solution[i].first.second<<" "<<solution[i].second<<endl;
		total_cost += tm[solution[i].first.first][solution[i].first.second]*solution[i].second;
	}
	cout<<"\nTotal Cost = "<<total_cost<<endl;
	
}
/*
4
4
6 3 1 4
7 6 2 1
10 4 5 9
7 7 7 3
50 55 75 60
90 65 30 55

3
3
6 8 10
7 11 11
4 5 12
150 175 275
200 100 300

5
5
70 37 6 76 17
59 90 93 5 10
93 62 77 47 62
54 55 26 9 84
53 20 84 15 9
18 17 19 13 15
16 18 20 14 14
 
4
4
6 3 1 4
7 6 8 2
10 4 5 9
7 7 7 3
50 55 75 60
90 65 30 55
*/

