// g++ -std=c++11 robot_path_reader.cpp  -o robot_path_reader
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#define Pi 3.1415926


using namespace std;

int main(){


    fstream myfile("/home/relaybot/api_ws/src/operation/src/ppl_src/robot_path.txt", ios_base::in);
    ofstream ofile;
    ofile.open("/home/relaybot/api_ws/src/operation/src/ppl_src/robot_path_map.txt");
    float ft_line;
	int num;
    int count = 0;
    if(myfile.is_open())
	{
        cout << " txt opened" << endl;
		myfile >> ft_line;
		num=ft_line;
		cout<<"num:"<<num<<endl;
		for(int i=0;i<num;i++)
        {   printf("\nrow %d: ",i);
			for(int j=0;j<4;j++){
                
                myfile>>ft_line;
                printf(" column %d: %f",j,ft_line);
		    }
            //x,y,w,z


            // ofile << robot_path[i].x << " " << robot_path[i].y << " " << w[i] << " " << z[i] << endl;

    
        }
        cout<<endl;		
    }
    myfile.close();
    ofile.close();


    return 0;
}
