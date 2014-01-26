#pragma once
//License: Public Domain
//Author: elf
//EMail: elf198012@gmail.com

template<typename T>
class SharedObject;

template<typename T>
class ObjectUser{
public:
 ObjectUser(){};

 ObjectUser(const ObjectUser &other)
  : _pObject(other._pObject){
  //fprintf(stderr, "Copy constructor\n");
  Reference();
 }

 ~ObjectUser(){Dereference();}

 ObjectUser &operator=(const ObjectUser &other){
  //fprintf(stderr, "operator=()\n");
  Dereference();
  _pObject = other._pObject;
  Reference();
  return *this;
 }

 bool SameObject(const ObjectUser &other)const{
  return _pObject == other._pObject;
 }

 bool operator==(const ObjectUser &other)const{
  return Object() == other.Object();
 }

 bool operator!=(const ObjectUser &other)const{
  return Object() != other.Object();
 }

//for container
 bool operator<(const ObjectUser &other)const{
  return Object() < other.Object();
 }

 bool operator>(const ObjectUser &other)const{
  return Object() > other.Object();
 }

 bool operator<=(const ObjectUser &other)const{
  return Object() <= other.Object();
 }

 bool operator>=(const ObjectUser &other)const{
  return Object() >= other.Object();
 }

 T &Object()const{
  return _pObject->Object();
 }

private:
 ObjectUser(SharedObject<T> *pObject): _pObject(pObject){
//  fprintf(stderr, "ObjectUser(SharedObject<T> *pObject)\n");
 }

 void Reference(){
  if(_pObject)
   _pObject->Reference();
 }

 void Dereference(){
  if(_pObject)
   _pObject->Dereference();
 }
 SharedObject<T> *_pObject = nullptr;
 friend class SharedObject<T>;
};

template<typename T>
class SharedObject{
public:
 static ObjectUser<T> New(const T &object){
  return ObjectUser<T>(new SharedObject(object));
 }

 void Reference(){
  ++_nCount;
  //fprintf(stderr, "Reference: %lu\n", _nCount);
 }

 void Dereference(){
  //fprintf(stderr, "Dereference: %lu\n", _nCount);
  if(--_nCount == 0)
   delete this;
 }

 T &Object(){
  return _object;
 }

private:
 SharedObject(const T &object): _object(object){}
 ~SharedObject(){}

 T _object;
 size_t _nCount = 1;
};
