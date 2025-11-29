#include <bits/stdc++.h>
using namespace std;

// all cases n*logn

void merge(int arr[], int l, int m, int r){

    int i, j;
    int n1 = m-l+1, n2 = r-m;

    int L[n1], R[n2];
    for(i=0; i<n1; i++)
        L[i] = arr[l+i];
    
    for(i=0; i<n2; i++)
        R[i]= arr[m+1+i];
    
    //merge functioning
    i=0; j=0;
    while(i<n1 && j<n2){
        if(L[i] <= R[j]){
            arr[l+i+j] = L[i];
            i++;
        }
        else{
            arr[l+i+j] = R[j];
            j++;
        }
    }

    //adds remaining elements from subarrays
    while(i<n1){
        arr[l+i+j] = L[i];
        i++;
    }

    while(j<n2){
        arr[l+i+j] = R[j];
        j++;
    }
}

void mergeSort(int arr[], int l, int r){

    if(l<r){
        int m = l + (r-l)/2;

        mergeSort(arr, l, m);
        mergeSort(arr, m+1, r);

        merge(arr, l, m, r);
    }
}

int main(){
    int arr[]={1, 3, 8, 2, 9, 2, 5, 6};
    int size = *(&arr + 1) - arr, i;

    for(i=0; i<size; i++)
    cout << arr[i] << " ";

    cout<<endl;
    
    mergeSort(arr, 0, size-1);

    for(i=0; i<size; i++)
    cout << arr[i] << " ";

}