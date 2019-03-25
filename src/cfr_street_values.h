#ifndef _CFR_STREET_VALUES_H_
#define _CFR_STREET_VALUES_H_

#include <memory>

#include "cfr_value_type.h"

class Buckets;
class Node;
class Reader;
class Writer;

class AbstractCFRStreetValues {
 public:
  virtual ~AbstractCFRStreetValues(void) {}
  virtual void AllocateAndClear(Node *node) = 0;
  virtual void RMProbs(int p, int nt, int offset,  int num_succs, int dsi, double *probs) const = 0;
  virtual void PureProbs(int p, int nt, int offset, int num_succs, double *probs) const = 0;
  virtual void Floor(int p, int nt, int num_succs, int floor) = 0;
  virtual bool Players(int p) const = 0;
  virtual void ReadNode(Node *node, Reader *reader, void *decompressor) = 0;
  virtual void WriteNode(Node *node, Writer *writer, void *compressor) const = 0;
  virtual CFRValueType MyType(void) const = 0;
  virtual void MergeInto(Node *full_node, Node *subgame_node, int root_bd_st, int root_bd,
			 const AbstractCFRStreetValues *subgame_values, const Buckets &buckets);
};

template <typename T>
class CFRStreetValues : public AbstractCFRStreetValues {
public:
  CFRStreetValues(int st, const bool *players, int num_holdings, int **num_nonterminals);
  virtual ~CFRStreetValues(void);
  CFRValueType MyType(void) const;
  bool Players(int p) const {return players_[p];}
  T *AllValues(int p, int nt) {return data_[p][nt];}
  void AllocateAndClear2(Node *node);
  void AllocateAndClear(Node *node);
  void RMProbs(int p, int nt, int offset, int num_succs, int dsi, double *probs) const;
  void PureProbs(int p, int nt, int offset, int num_succs, double *probs) const;
  void Floor(int p, int nt, int num_succs, int floor);
  void Set(int p, int nt, int h, int num_succs, T *vals);
  void InitializeValuesForReading(int p, int nt, int num_succs);
  void ReadNode(Node *node, Reader *reader, void *decompressor);
  void WriteNode(Node *node, Writer *writer, void *compressor) const;
  void MergeInto(Node *full_node, Node *subgame_node, int root_bd_st, int root_bd,
		 const CFRStreetValues<T> *subgame_values, const Buckets &buckets);
protected:
  int st_;
  std::unique_ptr<bool []> players_;
  int num_holdings_;
  std::unique_ptr<int []> num_nonterminals_;
  T ***data_;
};

#endif