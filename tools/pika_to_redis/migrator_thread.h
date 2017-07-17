#ifndef MIGRATOR_THREAD_H_
#define MIGRATOR_THREAD_H_

#include <iostream>
#include "nemo.h"
#include "pink/include/redis_cli.h"
#include "sender.h"

class MigratorThread : public pink::Thread {
 public:
  MigratorThread(nemo::Nemo *db, std::vector<Sender *> senders, char type, int thread_num) :
      db_(db),
      senders_(senders),
      type_(type),
      thread_num_(thread_num),
      thread_index_(0),
      num_(0)
      {
      }

  // prase key and generate cmd
  void SetTTL(const std::string &key, int64_t ttl);
  void ParseKey(const std::string &key,char type);
  void ParseKKey(const std::string &key);
  void ParseHKey(const std::string &key);
  void ParseSKey(const std::string &key);
  void ParseZKey(const std::string &key);
  void ParseLKey(const std::string &key);

  int64_t num() {
    slash::MutexLock l(&num_mutex_);
    return num_;
  }
  
  virtual ~ MigratorThread();
  bool should_exit_;
 private:
  nemo::Nemo *db_;
  //std::vector<ParseThread*> parsers_;
  std::vector<Sender *> senders_;
  char type_;
  int thread_num_;
  int thread_index_;

  static std::string GetKey(const rocksdb::Iterator *it);
  void MigrateDB(const char type);
  void DispatchKey(const std::string &key);

  int64_t num_;
  slash::Mutex num_mutex_;

  void PlusNum() {
    slash::MutexLock l(&num_mutex_);
    ++num_;
  }
  virtual void *ThreadMain();
};
#endif
