#ifndef BILLVECTOR_H
#define BILLVECTOR_H

#include <iostream>
#include <vector>
#include <initializer_list>
#include <stdexcept>
#include <cmath>
#include <type_traits>

namespace bill{

  template<typename T,size_t N>
  class vectorNd{
  static_assert(std::is_same<double,T>::value ||
		std::is_same<float,T>::value ||
		std::is_same<int,T>::value ||
		std::is_same<unsigned int,T>::value ||
		std::is_same<long,T>::value ||
		std::is_same<long long,T>::value,	"T must inherit from list");
  protected:
	std::vector<T> body;
  public:

    vectorNd<T,N>(){
		body=std::vector<T>(N);
    }
    vectorNd<T,N>(T value){
		body=std::vector<T>(N,value);
    }
    vectorNd<T,N>(std::initializer_list<T> list){
  		if(list.size() != N){
  		  std::cerr<<("bill::vector<T,N>: initialization list is not the size of vector! Additional numbers are removed!\n");
		}
		body=std::vector<T>(list);
  		if(list.size() != N){
		  body.resize(N);
		}
    }
    vectorNd<T,N>(std::vector<T> vec){
		if(vec.size()!=N)
		   std::cerr<<("bill::vector<T,N>: initialization vector is not the size of vector! Additional numbers are removed!\n");
		body=std::vector<T>(vec);
		if(vec.size() != N){
		  body.resize(N);
		}
    }

    // FUNCTIONS
    
    size_t size() const{return body.size();}
    
    vectorNd versor(){
    //std::cout<<norm(*this)<<std::endl;
      return (*this)/norm(*this);
      
    }
    
    void normalize(){
     T mynorm=norm(*this);
     for (auto& it : body)
       it /= mynorm;
    }
    
    
    // OPERATORS
    
    T operator[](size_t n) const{
		if(n>=N || n<0){
		   std::cerr<<("bill::vector<T,N>::operator[]: there is no field to access!\n");
		   return T();
		}
     return body[n]; 
    }
    
    T& operator[](size_t n){
		if(n>=N || n<0){
		   std::cerr<<("bill::vector<T,N>::operator[]: there is no field to access!\n");
		   return *(new T);
		}
     return body[n]; 
    }
    
    std::vector<T> operator()(){
     return body; 
    }
    
    
     vectorNd operator+(const vectorNd & right) const {
 		if(right.size()!=N)
 		   std::cerr<<("bill::vector<T,N>::operator+: vectors are uneven!\n");
        std::vector<T>sum;
	for(unsigned int i=0; i<N; i++)
	    sum.push_back(body[i]+right[i]);
        return vectorNd(sum);
    }
     vectorNd operator-(const vectorNd & right) const {
		if(right.size()!=N)
		   std::cerr<<("bill::vector<T,N>::operator-: vectors are uneven!\n");
        std::vector<T>sum;
	for(unsigned int i=0; i<N; i++)
	    sum.push_back(body[i]-right[i]);
        return vectorNd(sum);
    }
    vectorNd operator-() const{
      std::vector<T>sum;
	for(unsigned int i=0; i<N; i++)
	    sum.push_back(-body[i]);
        return vectorNd(sum);
    }
    void operator+=(const vectorNd & right) {
	for(unsigned int i=0; i<N; i++)
	    body[i]+=right[i];
    }
    void operator-=(const vectorNd & right) {
	for(unsigned int i=0; i<N; i++)
	    body[i]-=right[i];
    }
     double operator*(const vectorNd & right) const {
	if(right.size()!=N)
		   std::cerr<<("bill::vector<T,N>::operator-: vectors are uneven!\n");

        double sum=0;
	for(unsigned int i=0; i<N; i++)
	    sum+=(body[i]*right[i]);
        return sum;
    }
     vectorNd operator^(const vectorNd & right) const {
 		if(right.size()!=N){
 		   std::cerr<<("bill::vector<T,N>::operator+: vectors are uneven!\n");
		   return vectorNd<T,N>();
		}
		if(N!=3){
		    std::cerr<<("bill::vector<T,N>::operator^: vector product exist only in 3 dimensions!\n");
		    return vectorNd<T,N>();
		}
		
        std::vector<T>product(3);
	
	product[0]=body[1]*right[2]-body[2]*right[1];
	product[1]=body[2]*right[0]-body[0]*right[2];
	product[2]=body[0]*right[1]-body[1]*right[0];
	
        return vectorNd(product);
     }
    vectorNd operator/(const T & scalar) const {
        std::vector<T>sum(N);
	for(unsigned int i=0; i<N; i++)
	    sum[i]=body[i]/scalar;
        return vectorNd(sum);
    }
    vectorNd operator*(const T & scalar) const {
        std::vector<T>sum(N);
	for(unsigned int i=0; i<N; i++)
	    sum[i]=body[i]*scalar;
        return vectorNd(sum);
    }
    friend vectorNd operator*(const T & scalar, const vectorNd& right) {
        return right*scalar;
    }
   void operator*=(const T & scalar) {
        for(auto& item : body)
	    item*=scalar;
    }
    void operator/=(const T & scalar) {
        for(auto& item : body)
	    item/=scalar;
    }

    
	friend std::ostream& operator<<(std::ostream& stream, const vectorNd& vector){
	 stream<<"[";
	 for (const auto& item : vector.body)
	   stream<<item<<", ";
	 stream<<"\b\b]";
	 
	 return stream;
	}
	
	// STATIC FUNCTIONS
	static double square_form(const vectorNd& vector){
	  T localnorm=T(0);
	   //std::cout<<"+-------------+"<<std::endl;
	  for(const auto& item : vector.body){
	    localnorm += item*item;
	   // std::cout<<item<<" "<<localnorm<<std::endl;
	  }
	   //std::cout<<"+-------------+"<<std::endl;
	  return localnorm;
	}
	
	static T norm(const vectorNd& vector){
	  return sqrt(square_form(vector));
	}
	
  };	//end class vectorNd
  
  using vector  = vectorNd<double,3U>;
  using vectorf = vectorNd<float,3U>;
  using vectori = vectorNd<int,3U>;
  using vectorU = vectorNd<unsigned int,3U>;
  using vectorl = vectorNd<long,3U>;
  using vectorll= vectorNd<long long,3U>;

  class quaternion{
  protected:
    double s;
    vector v;
  public:
    quaternion():s(1.0){
    }
    quaternion(double value):s(value),v(value){
    }
    quaternion(double valueV, double valueS):s(valueS),v(valueV){
    }
    quaternion(std::initializer_list<double> list){
  		if(list.size() == 3){
		  v = vector(list);
		  s = 1.0;
		}
		if(list.size() == 4){
		  std::vector<double> tmp;
		  tmp.insert(tmp.end(),list.begin(),list.end());
		  s = tmp[0];		// first element is scalar
		  tmp.erase(tmp.begin()); // remove first element
		  v = vector(tmp);
		}
  		if(list.size() > 4){
		  std::cerr<<("bill::quaternion: initialization list is not the size of vector! Additional numbers are removed!\n");
		  std::vector<double> tmp;
		  tmp.insert(tmp.end(),list.begin(),list.begin()+4);
		  s = tmp[0];		// first element is scalar
		  tmp.erase(tmp.begin()); // remove first element
		  v = vector(tmp);
		}
    }
    quaternion(double valueS, std::initializer_list<double> list):s(valueS){
  		if(list.size() == 3){
		  v = vector(list);
		}
  		if(list.size() > 3){
		  std::cerr<<("bill::quaternion: initialization vec is not the size of vector! Additional numbers are removed!\n");
		  v = vector(list);
		}
    }
    quaternion(std::vector<double> vec){
    		if(vec.size() == 3){
		  v = vector(vec);
		  s = 1.0;
		}
		if(vec.size() == 4){
		  s = vec[0];		// first element is scalar
		  std::vector<double> tmp;
		  tmp.insert(tmp.end(),vec.begin()+1,vec.end());
		  v = vector(tmp);
		}
  		if(vec.size() > 4){
		  std::cerr<<("bill::quaternion: initialization vec is not the size of vector! Additional numbers are removed!\n");
		  s = vec[0];		// first element is scalar
		  v = vector(vec);
		}
    }
    quaternion(double valueS, std::vector<double> vec):s(valueS){
  		if(vec.size() == 3){
		  v = vector(vec);
		}
  		if(vec.size() > 3){
		  std::cerr<<("bill::quaternion: initialization vec is not the size of vector! Additional numbers are removed!\n");
		  v = vector(vec);
		}
    }
    quaternion(double valueS,vector valueV):s(valueS),v(valueV){
    }
    
    quaternion(vector versor, double theta):s(cos(theta/2.0)),v(versor.versor()*sin(theta/2.0)){
    }
    
    double operator[](int n) const{
		if(n>=3 || n<-1){
		   std::cerr<<("bill::quaternion::operator[]: there is no field to access!\n");
		   return 0.0;
		}
		else if (n==-1)
		  return s;
		else
		  return v[n]; 
    }
    
    double& operator[](int n){
		if(n>=3 || n<-1){
		   std::cerr<<("bill::quaternion::operator[]: there is no field to access!\n");
		   return *(new double);
		}
		else if (n==-1)
		  return s;
		else
		  return v[n]; 
    }    
     
    vector& operator[](char c) {
		return v; 
    }
    
    quaternion operator+(const quaternion & right) const {
      return quaternion(this->s + right.s, this->v + right.v);
    }
    quaternion operator-(const quaternion & right) const {
      return quaternion(this->s - right.s, this->v - right.v);
    }
    quaternion operator*(const quaternion & right) const {
        return quaternion( this->s*right.s - this->v*right.v,
			  (this->s*right.v) + (this->v*right.s) 
					    + (this->v^right.v));
    }
    
    quaternion operator/(const double & scalar) const {
        return quaternion(s/scalar,v/scalar);
    }
    quaternion operator*(const double & scalar) const {
        return quaternion(s*scalar,v*scalar);
    }
    friend quaternion operator*(const double & scalar, const quaternion& right) {
        return right*scalar;
    }
    
    quaternion operator-() const{
      return quaternion(-s,-v);
    }
	
    void operator/=(const double & scalar) {
        s /= scalar;
	v /= scalar;
    }
    void normalize(){
     double n = norm(*this);
     s /= n;
     v /= n;
    }
    quaternion& operator*() const{	// conjugate quaternion
	return *(new quaternion(this->s,-(this->v)));
    }
    quaternion operator!() const{	// resiprocal quaternion
	return *(*this)/quaternion::square_form(*this);
    }
    
    static double square_form(quaternion q) {
	return q.s*q.s + vector::square_form(q.v);
    }
    static double norm(quaternion q) {
	return sqrt(square_form(q));
    }
    static void rotate(vector& vec, const vector& axis, const double& angle){
	 bill::quaternion q (axis,angle);
	 vec = (q*bill::quaternion(0.0,vec)*(!q))['v'];
    }
    void rotateMe(vector& vec){
	 vec = ((*this)*bill::quaternion(0.0,vec)*(!(*this)))['v'];
    }
    vector rotate(vector vec){
	  return ((*this)*bill::quaternion(0.0,vec)*(!(*this)))['v'];
    }
    void update(vector vec){
      v=vec;
    }
    void update(double scalar){
      s=scalar;
    }
    void updateU(vector vec){
      double th2=acos(s);
      v = sin(th2)*(vec.versor());
    }
    void updateU(double scalar){
      double th2=acos(s);
      s = cos(0.5*scalar);
      v/= sin(th2);
      v*= sin(0.5*scalar);
    }
    
	friend std::ostream& operator<<(std::ostream& stream, const quaternion& q){
	 stream<<q.s<<" | "<<q.v;
	  return stream;
	}
  };	// end class quaternion
  
}	//end namespace bill
#endif //BILLVECTOR_H
