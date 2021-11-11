class Items {
public:
  virtual void preupdate() {};
  virtual void postupdate() {};
  virtual void on_recv_damage() {};
  virtual void on_dealt_damage() {};
};
