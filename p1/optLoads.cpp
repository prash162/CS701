//===--------------- optLoads.cpp - Project 1 for CS 701 ---------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file is a skeleton of an implementation for the optLoads
// pass of Univ. Wisconsin-Madison's CS 701 Project 1.
//
//===----------------------------------------------------------------------===//

#define DEBUG_TYPE "optLoads"
#include "llvm/Pass.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Instruction.h"
#include "llvm/ADT/StringExtras.h"
#include "llvm/ADT/Statistic.h"
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <list>
using namespace llvm;

namespace {
  class optLoads : public FunctionPass {
    private:
    static int insNum;
    static int i;
    std::map<Instruction*,int> insMap;
 
    private:
     void fillInsMap(Function *F){
          for (Function::iterator B = F->begin(), e = F->end(); B!=e; ++B) {
                  for (BasicBlock::iterator j= B->begin(), f= B->end(); j!=f; ++j){
                          Instruction* Ins = j;
                          insMap.insert( std::pair<Instruction*,int>( Ins,insNum));
                          insNum++;
                   }
           }
      }


    public:
    static char ID; // Pass identification, replacement for typeid
    optLoads() : FunctionPass(ID) {}
    
    //**********************************************************************
    // runOnFunction
    //**********************************************************************
    virtual bool runOnFunction(Function &F) {
     bool changeMade = false;
     fillInsMap(&F);

    std::list<Instruction*> loadRemoveList;
     for (Function::iterator B = F.begin(), e = F.end(); B!=e; ++B) {
	for (BasicBlock::iterator j= B->begin(), prev=j++, f= B->end(); j!=f; ++j,++prev){
		if( strncmp(prev->getOpcodeName(),"store",5)==0  && strncmp(j->getOpcodeName(),"load",4)==0  ){
                     if(prev->getOperand(1)==j->getOperand(0)){
			std::cerr<< "%"<<insMap[j] << " is a useless load\n";
			j -> replaceAllUsesWith(prev->getOperand(0));
			loadRemoveList.push_back(j);
			changeMade = true;
		    }
		}	
	}
  }

	
	
	while(!loadRemoveList.empty()){
		Instruction* rem =  loadRemoveList.front();
		rem->eraseFromParent(); 
		loadRemoveList.pop_front();
	}



	if(changeMade == true){
	      return true;
	}
	else		  
	      return false;  // because we have NOT changed this function
    }

    //**********************************************************************
    // print (do not change this method)
    //
    // If this pass is run with -f -analyze, this method will be called
    // after each call to runOnFunction.
    //**********************************************************************
    virtual void print(std::ostream &O, const Module *M) const {
        O << "This is optLoads.\n";
    }

    //**********************************************************************
    // getAnalysisUsage
    //**********************************************************************

    // We don't modify the program, so we preserve all analyses
    virtual void getAnalysisUsage(AnalysisUsage &AU) const {
      AU.setPreservesAll();
    };

  };
  int optLoads::insNum=1;
  char optLoads::ID = 0;
  int optLoads::i = 0;

  // register the optLoads class: 
  //  - give it a command-line argument (optLoads)
  //  - a name ("print code")
  //  - a flag saying that we don't modify the CFG
  //  - a flag saying this is not an analysis pass
  RegisterPass<optLoads> X("optLoads", "optimize unnecessary loads",
			   false, false);
}
