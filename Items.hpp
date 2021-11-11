class Items {
public:
  virtual void preupdate() {};
  virtual void postupdate() {};
  virtual void on_recv_damage() {};
  virtual void on_dealt_damage() {};
  virtual void on_kill() {};
  virtual void on_bullet_destroyed() {};
};
