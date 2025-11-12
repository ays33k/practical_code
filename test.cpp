#include<bits/stdc++.h>
using namespace std;

int main(){
    int n;
    cout<<"Length of reference string ";
    cin >> n;
    cout<<"enter page reference string ";
    int a[n];
    for (int i = 0; i < n; i++) {
        cin >> a[i];
    }
    int hit=0,miss=0,j=0;
    int slot[3] = {-1, -1, -1};

    // fifo
    for (int i = 0; i < n; i++) {
        bool found=false;
        for (int k = 0; k < 3; k++) {
            if(a[i] == slot[k]){
                found=true;
                hit+=1;
                break;
            }
        }
        if(!found){
            miss++;
            slot[j%3]=a[i];
            j++;
        }
    }
    cout<<"hit:-"<<hit<<" "<<"miss:-"<<miss<<endl;
    // optimal 
    hit=0,miss=0;
    for(auto &it:slot) it=-1;
    for (int i = 0; i < n; i++) {
        bool found=false;
        for (int k = 0; k < 3; k++) {
            if(slot[k]==a[i]){
                found =true;
                hit+=1;
                break;
            }
        }
        if(!found){
            miss++;
            bool emp=false;
            for(int k=0 ; k<3 ; k++){
                if(slot[k]==-1){
                    slot[k]=a[i];
                    emp=true;
                    break;
                }
            }
            if(emp) continue;
            int far=-1,val=-1;
            for(int f=0;f<3;f++){
                int fin=INT_MAX;
                for(int p=i+1;p<n ; p++){
                    if(a[p]==slot[f]){
                        fin=p;
                        break;
                    }
                }
                if(fin==INT_MAX){
                    val=f;
                    break;
                }
                if(fin>far){
                    far=fin;
                    val=f;
                }
            }
            slot[val]=a[i];
        }
    }

    // lru
    for(auto &it:slot) it=-1;
    int lru[3]  = {-1, -1, -1};     
    hit=0,miss=0;
    for (int i = 0; i < n; i++) {
        bool found=false;
        for (int k = 0; k < 3; k++) {
            if(a[i]==slot[k]){
                hit+=1;
                lru[k]=i;
                found=true;
                break;
            }
        }
        if(!found){
            miss+=1;
            int lu=0;
            for (int j = 0; j < 3; j++) {
                if(lru[j]<lru[lu]){
                    lu=j;
                }
            }
            slot[lu]=a[i];
            lru[lu]=i;
        }
    }

}