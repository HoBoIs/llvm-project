#ifndef LLVM_CLANG_SEMA_CACHE_HPP
#define LLVM_CLANG_SEMA_CACHE_HPP
#include "clang/AST/Type.h"
#include "clang/Sema/Overload.h"
#include <cstddef>
#include <unordered_map>
namespace clang{
class Cache{
  struct Key{
    const QualType lhs,rhs;
    const int CombinedData;
    const unsigned int size,AdlSize;
    std::size_t getHash()const{
      return size_t (lhs.getAsOpaquePtr()) ^
        (size_t(rhs.getAsOpaquePtr()) << 1) ^
        (size_t(CombinedData+(size_t(size)<<32)) <<2) ^
        (size_t(AdlSize) <<3);
    };

    Key(const Expr * const Args[2],OverloadedOperatorKind K,bool CanRewrite,size_t s,size_t AdlS,const UnresolvedSetImpl& Fns,DeclarationName rewritenOp):
                          lhs(Args[0]->getType().getCanonicalType()),
                          rhs(Args[1]->getType().getCanonicalType()),
                          CombinedData(CanRewrite | 
                                (Args[0]->getValueKind()<<1) | 
                                (Args[1]->getValueKind()<<3) |
                                ((lhs->getAsCXXRecordDecl()&&lhs->getAsCXXRecordDecl()->hasDefinition())<<5)|
                                ((CanRewrite &&
                                 rhs->getAsCXXRecordDecl()&&rhs->getAsCXXRecordDecl()->hasDefinition())<<6)|
                                (K<<7)),
                          size(s),AdlSize(AdlS){
                          //fst(Fns.size()?(NamedDecl*)Fns[0]:nullptr),fstRewriten(getFirstRewritten(Fns, rewritenOp)){
                            //lhs->getAsCXXRecordDecl()->methods().begin()-lhs->getAsCXXRecordDecl()->methods().begin();
                            static_assert(OverloadedOperatorKind::NUM_OVERLOADED_OPERATORS < std::numeric_limits<int>::max()/(1<<7),"Why do we have this many operators? This in nonsense!");
                          };
    bool operator==(const Key& o)const{
      return lhs==o.lhs && rhs==o.rhs && CombinedData==o.CombinedData && 
             size==o.size && AdlSize==o.AdlSize;
      // && lk==o.lk && rk ==o.rk && Kind==o.Kind && AllowRewritten==o.AllowRewritten;
    }
  };
  struct CacheHash{
    size_t operator()(const Key& val)const{
      return val.getHash();
    };
  };

  struct Value{
    llvm::SmallVector<ImplicitConversionSequence,2> ICS;
    OverloadingResult res;
    bool hadMultipleCandidates;
  };
  struct Validation{
    const NamedDecl* fst, *fstRewriten;
  };
  std::unordered_map<Key, std::pair<Validation,Value>> mp;
  public:
  bool compute(OverloadCandidateSet& OCs, Sema& S,const Key& key){
    auto it=mp.find(key);
    if (it==mp.end()){

    }else {
    
    }
  }
};
} // namespace clang
#endif
