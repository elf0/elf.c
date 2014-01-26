#pragma once
//License: Public Domain
//Author: elf
//EMail: elf198012@gmail.com

template<typename T>
class SharedObject{
public:
 static SharedObject *New(){
  return new SharedObject();
 }

 void Reference(){
  ++_nCount;
//  fprintf(stderr, "Reference: %lu\n", _nCount);
 }

 void Dereference(){
//  fprintf(stderr, "Dereference: %lu\n", _nCount);
  if(--_nCount == 0)
   delete this;
 }

 T &operator*(){
  return _object;
 }

private:
 SharedObject(){}
 ~SharedObject(){}

 T _object;
 size_t _nCount = 1;
};

template<typename T>
class ObjectUser{
protected:
 ObjectUser(){};
 ObjectUser(SharedObject<T> *pObject): _pObject(pObject){}

 ObjectUser(const ObjectUser &other)
  : ObjectUser(other._pObject){
  Reference();
 }

 ObjectUser &operator=(const ObjectUser &other){
  if(_pObject == other._pObject)
   return *this;

  Dereference();
  _pObject = other._pObject;
  Reference();
  return *this;
 }

 ~ObjectUser(){Dereference();}

 T &Object()const{
  return **_pObject;
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
