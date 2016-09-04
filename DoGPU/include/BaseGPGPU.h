#ifndef BASEGPGPU_H
#define BASEGPGPU_H

//Abstract base class for either Compute shader class or Transform feedback

namespace doGPU
{
  class BaseGPGPU
  {

  public:
    BaseGPGPU();
    ~BaseGPGPU();

    void base_initialize();
    void base_sendData();
    void base_use();

  private:

  };
}
#endif // BASEGPGPU_H
