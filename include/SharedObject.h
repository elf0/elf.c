#pragma once
//License: Public Domain
//Author: elf
//EMail: elf198012@gmail.com

template<typename T>
class SharedObject;

template<typename T>
class ObjectUser{
//protected:
public:
 ObjectUser(){};

 ObjectUser(SharedObject<T> *pObject): _pObject(pObject){
//  fprintf(stderr, "ObjectUser(SharedObject<T> *pObject)\n");
 }

 ObjectUser(const ObjectUser &other)
  : _pObject(other._pObject){
  //fprintf(stderr, "Copy constructor\n");
  Reference();
 }

 ObjectUser &operator=(const ObjectUser &other){
  //fprintf(stderr, "operator=()\n");
  if(_pObject == other._pObject)
   return *this;

  Dereference();
  _pObject = other._pObject;
  Reference();
  return *this;
 }

 ~ObjectUser(){Dereference();}

 T &Object()const{
  return _pObject->Object();
 }

private:
 void Reference(){
  if(_pObject)
   _pObject->Reference();
 }

 void Dereference(){
  if(_pObject)
   _pObject->Dereference();
 }
 SharedObject<T> *_pObject = nullptr;
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
