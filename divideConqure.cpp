/* 
    Algoritma Analizi programlama ödevi 1 
    Emirhan Aktürk 
    19120205058
    
    Problemi kenarlara parçalarayak önce kenarların çözümü ,sonra birleştirilmesi ile tüm sonuca ulaşılmıştır.
    Problemin çözümü için Divide and Conquer yöntemi kullanılmıştır

    (dörtgen sayisi=row=n)
    getRow:O(n) 
    fread:O(n)
    printLines:O(n)
    updateXlines:O(n^2)
    updateYlines:O(n^2)
    length:O(n)
    Total run time:O(n^2)
*/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

typedef struct{
    int x,y;
}point;

typedef struct{
    point p1;
    point p2;
}Line;

int  xLength,yLength;
static int maxEnd;
static int minStart=1000;//max deger ile ilklendirmek için

/* 
length:for döngüsü yLines.size() kadar dönecek.yLines.size()<=2*row olduğuna göre row=n dersek
Çalışma zamanı O(n)  */
int length(const vector<Line> &xLines,const vector<Line> &yLines ){
//    xLength=maxEnd-minStart;
    for(int i=0;i<xLines.size();++i){
        xLength+=(xLines[i].p2.x-xLines[i].p1.x);
    }

    for(int i=0;i<yLines.size();++i){
        yLength+=(yLines[i].p2.y-yLines[i].p1.y);
    }
    cout<<"xlength:"<<xLength<<" ylength:"<<yLength<<endl;
    return (xLength+yLength);
}

/* 
sortLines:for döngüsü Lines.size() kadar,while döngüsü kendinden küçük eleman bulana kadar devam edecek.
Lines.size()<=2*row olduğuna göre row=n dersek
Average case = Worst case = O(n^2)
 */
void sortLines(vector<Line>&Lines){//insertion sort
    int i, j;  
    Line key;
    for (i = 1; i < Lines.size(); i++) 
    {  
        key = Lines[i];  
        j = i - 1;  

        while (j >= 0 && Lines[j].p1.x> key.p1.x) 
        {  
            Lines[j + 1] = Lines[j];  
            j = j - 1;  
        }  
        Lines[j + 1] = key;  
    }  
} 

/* 
getRow:while döngüsü dosyadaki karakter sayısı kadar dönecek.
Karakter sayısı=row*(c sabiti)    (row=n)
Çalışma süresi O(n).
 */
int getRow(){
    ifstream ip("data.csv");
    if(!ip.is_open()) cout<<"ERROR:File Open"<<endl;
    char c;
    int row=1;
     while (ip.get(c)){
      if(c=='\n')
        ++row;
    }

    ip.close();
    return row;
}

/* 
fread:for döngüsü row kadar dönecek. row=n  dersek çalışma süresi:O(n)
 */
void fread(vector<Line> &xLines,vector<Line> &yLines){
    int row=xLines.size();
    ifstream ip("data.csv");
    if(!ip.is_open()) cout<<"ERROR:File Open"<<endl;

    for(int i=0;i<row;++i){
        string height,weight,start;
        int h,w,s;
        int end;
        getline(ip,weight,';');
        getline(ip,height,';');
        getline(ip,start,'\n');
        w=stoi(weight);
        h=stoi(height);
        s=stoi(start);
        
        end=s+w;

        if(end>maxEnd)
            maxEnd=end;

        if(s<minStart)
            minStart=s;

        xLines[i].p1.x=s;
        xLines[i].p1.y=h;
        xLines[i].p2.x=s+w;
        xLines[i].p2.y=h;

        yLines[i].p1.x=s;
        yLines[i].p1.y=0;
        yLines[i].p2.x=s;
        yLines[i].p2.y=h;

        yLines[row+i].p1.x=s+w;
        yLines[row+i].p1.y=0;
        yLines[row+i].p2.x=s+w;
        yLines[row+i].p2.y=h;

    }
    Line newLine;
    newLine.p1.x=minStart;
    newLine.p1.y=0;
    newLine.p2.x=maxEnd;
    newLine.p2.y=0;
    xLines.push_back(newLine);

    ip.close();
}

/* printLines:lines.size() ~ 2*row .Çalışma süresi O(n) */
void printLines(const vector<Line>&lines){
    for(int i=0;i<lines.size();++i){
        cout<<i+1<<".line:("<<lines[i].p1.x<<","<<lines[i].p1.y<<"),"<<
        "("<<lines[i].p2.x<<","<<lines[i].p2.y<<")"<<endl; 
    }
}

/* updateXlines:xLines.size()=row.iç içe for toplamda row*row kadar dönecek.(row=n) için
Çalışma süresi: O(n^2)*/
void updateXlines(vector<Line>&xLines){
    for(int i=0;i<xLines.size();++i){
        for(int j=0;j<xLines.size();++j){
            if(j!=i){
                
                if(xLines[i].p1.x >= xLines[j].p1.x && xLines[i].p1.x <= xLines[j].p2.x ){//1. doğrunun başlangıcı 2. doğrunun yatay izdişümü içindeyse
                    
                    if(xLines[i].p2.x >= xLines[j].p2.x){//1. doğrunun bitişi 2. doğrunun bitişinin sağındaysa
                    
                        if(xLines[i].p1.y < xLines[j].p1.y){//1. doğru 2. doğrunun altındaysa
                            xLines[i].p1.x=xLines[j].p2.x;//1. doğrudaki 2.doğrunun altında kalan kısmı çıkar
                        }
                    
                        else{

                            xLines[j].p2.x=xLines[i].p1.x;//2. doğrudaki 1.doğrunun altında kalan kısmı çıkar
                            if(xLines[i].p2.x == xLines[i].p1.x){//uzunluk sıfırsa
                                xLines[i]=xLines.back();//1. doğruyu listeden çıkar
                                xLines.pop_back();//silme işlemi O(1) zamanda yapılıyor
                                --i; //listeden çıkarılan elemanın yerine geleni kontrol et
                            }        
                        }
                    }
                    
                    else{//1.doğru tamamen 2. doğrunun izdüşümündeyse
                    
                        if(xLines[i].p1.y < xLines[j].p1.y){//1. doğru 2. doğrunun altındaysa
                            xLines[i]=xLines.back();//1. doğruyu listeden çıkar
                            xLines.pop_back();//silme işlemi O(1) zamanda yapılıyor
                            --i; //listeden çıkarılan elemanın yerine geleni kontrol et
                        }
                    
                        else{//1.doğru 2. doğrunun izdüşümünde ve üstündeyse
                            Line newLine=xLines[j];
                            xLines[j].p2.x=xLines[i].p1.x;//2. doğrudaki 1.doğrunun altında kalan kısmı çıkar        
                            newLine.p1.x=xLines[i].p2.x;
                            xLines.push_back(newLine);//2. doğrudan kalan diğer parçayı listeye ekle
                        }
                    }
                }
                
                else if(xLines[i].p2.x >= xLines[j].p1.x && xLines[i].p2.x <= xLines[j].p2.x ){//1. doğrunun sonu 2. doğrunun yatay izdişümü içindeyse
                        
                        if(xLines[i].p2.y>xLines[j].p1.y){//1.doğru üstteyse 2.doğrunun altta kalan kısmını çıkar
                            xLines[j].p1.x=xLines[i].p2.x;//2.doğrunun başlangıcı=1.doğrunun sonu
                        }

                        else{//2.doğru üstteyse 1.doğrunun altta kalan kısmını çıkar
                            xLines[i].p2.x=xLines[j].p1.x;//1.doğrunun bitişi=2.doğrunun başlangıcı
                            if(xLines[i].p2.x == xLines[i].p1.x){//uzunluk sıfırsa
                                xLines[i]=xLines.back();//1. doğruyu listeden çıkar
                                xLines.pop_back();//silme işlemi O(1) zamanda yapılıyor
                                --i; //listeden çıkarılan elemanın yerine geleni kontrol et
                            }
                        }

                    }
    
            }
        }
    }
    sortLines(xLines);
}

/* 
updateYlines:xLines.size()=row  yLines.size()= 2*row.iç içe for toplamda 2*row*row kadar dönecek.
Döngüden sonraki sıralama fonksiyonu Average case O(n^2) de çalışıyor.(row=n) için 
toplam çalışma süresi O(n^2+n^2) = O(n^2)
*/
void updateYlines(vector<Line>&yLines,const vector<Line>&xLines){
    for(int i=0;i<yLines.size();++i){
        for(int j=0;j<xLines.size();j++){
                
            if(yLines[i].p2.x >= xLines[j].p1.x && yLines[i].p2.x <= xLines[j].p2.x ){//düşey doğru yatay doğrunun izdişümü içindeyse
                
                if( yLines[i].p2.y > xLines[j].p2.y ){//düşey doğrunun bitişi yatay doğrunun üstündeyse        
                    yLines[i].p1.y=xLines[j].p2.y; //düşey doğrudan yatay doğrunun altında kalan kısmını çıkar
                }
                
                else if(yLines[i].p2.y < xLines[j].p2.y){//düşey doğru tamamen yatay doğrunun altındaysa   
                    yLines[i]=yLines.back();//doğruyu listeden çıkar
                    yLines.pop_back();//silme işlemi O(1) zamanda yapılıyor
                    --i;//listede çıkarılan elemanın yerine geleni atlamamak için 
                }                                                                

            }
        }
    }
    sortLines(yLines);
}

int main(){
    int row=getRow();
    cout<<"Row:"<<row<<endl;
    vector<Line>xLines(row);
    vector<Line>yLines(2*row);
    fread(xLines,yLines);

    cout<<"\nxLines:"<<endl;
    printLines(xLines);

    updateXlines(xLines);

    cout<<"\nupdated xLines:"<<endl;
    printLines(xLines);
    
    cout<<"\nyLines:"<<endl;
    printLines(yLines);

    updateYlines(yLines,xLines);

    cout<<"\nupdated yLines:"<<endl;
    printLines(yLines);

    cout<<"\nlength of the rope:"<<length(xLines,yLines)<<endl;
    

    return 0;
}