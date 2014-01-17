#include <iostream>
#include <map>
#include <cassert>
#include <vector>

using namespace std;

#include "semantics.h"
#include "parsetree.h"

Symtab::Symtab(Symtab *p) : parent(p) {}

semantics * Symtab::lookup(string key) { 
  //return local_lookup(key) or parent and parent->lookup(key);
  semantics *answer;
  return 
    (answer = local_lookup(key)) ? answer : 
    parent ? parent->lookup(key) : NULL;

}



semantics * Symtab::local_lookup(string key) { 
  return dict[key];
}

void Symtab::insert(string key, semantics * item) {
  assert (dict[key] == NULL);
  dict[key] = item;
}

Symtab * currentScope = NULL;
int scopesOpened = 0;
int scopesClosed = 0;
int scopeDepth = 0;
void openscope()
{
  scopesOpened++;
  scopeDepth++;
  currentScope = new Symtab(currentScope);
}

Symtab *closescope()
{
  scopesClosed++;
  scopeDepth--;
  Symtab *v = currentScope;
  currentScope = currentScope->parent;
  return v;
}


