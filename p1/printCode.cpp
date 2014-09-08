//===--------------- printCode.cpp - Project 1 for CS 701 ---------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file is a skeleton of an implementation for the printCode
// pass of Univ. Wisconsin-Madison's CS 701 Project 1.
//
//===----------------------------------------------------------------------===//

#define DEBUG_TYPE "printCode"
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
using namespace llvm;
namespace {
  class printCode : public FunctionPass {
    private:
    static int insNum; 
    std::map<Instruction*,int> insMap;
   
    private:
    void fillInsMap(Function *F){

      		for (Function::iterator B = F->begin(), e = F->end(); B!=e; ++B) {
	      		for (BasicBlock::iterator j= B->begin(), f= B->end(); j!=f; ++j){
				insMap.insert( std::pair<Instruction*,int>( (j),insNum));
				insNum++;
			}
		}       
    }

    public:
    static char ID; // Pass identification, replacement for typeid
    printCode() : FunctionPass(ID) {}
    
    //**********************************************************************
    // runOnFunction
    //**********************************************************************
    virtual bool runOnFunction(Function &F) {
      
      fillInsMap(&F);   // Fill each Instruction in code with unique instruction ID
      
      // print fn name
      std::cerr << "FUNCTION " << F.getName().str() << "\n";
	
 	//Iterate over Basic Blocks	
      for (Function::iterator B = F.begin(), e = F.end(); B!=e; ++B) {
	      std::cerr << "\nBASIC BLOCK " << B->getName().str()<< "\n"; 
	  	
	      for (BasicBlock::iterator j= B->begin(), f= B->end(); j!=f; ++j){
		 std::cerr <<"%"<< insMap[j]<<":\t"<<j->getOpcodeName()<<"\t";    
          			
			for(User::op_iterator Op = j->op_begin(), En = j->op_end(); Op!=En; ++Op) {
				if(isa<Instruction>(Op))
					std::cerr << "%"<< insMap[(Instruction*)Op];

				else{
					if( ((Value*)Op)->hasName()){
						std::cerr <<  ((Value*)Op)->getName().str() << " ";
					}

					else {
						std::cerr << "XXX "; 
					}

				}			
	
			}
		std::cerr << "\n"; 
		}
     }


      // MISSING: Add code here to do the following:
      //          1. Iterate over the instructions in F, creating a
      //             map from instruction address to unique integer.
      //             (It is probably a good idea to put this code in
      //             a separate, private method.)
      //          2. Iterate over the basic blocks in the function and
      //             print each instruction in that block using the format
      //             given in the assignment.

      return false;  // because we have NOT changed this function
    }

    //**********************************************************************
    // print (do not change this method)
    //
    // If this pass is run with -f -analyze, this method will be called
    // after each call to runOnFunction.
    //**********************************************************************
    virtual void print(std::ostream &O, const Module *M) const {
        O << "This is printCode.\n";
    }

    //**********************************************************************
    // getAnalysisUsage
    //**********************************************************************

    // We don't modify the program, so we preserve all analyses
    virtual void getAnalysisUsage(AnalysisUsage &AU) const {
      AU.setPreservesAll();
    };

  };
  int printCode::insNum =1;
  char printCode::ID = 0;

  // register the printCode class: 
  //  - give it a command-line argument (printCode)
  //  - a name ("print code")
  //  - a flag saying that we don't modify the CFG
  //  - a flag saying this is not an analysis pass
  RegisterPass<printCode> X("printCode", "print code",
			   true, false);
}
