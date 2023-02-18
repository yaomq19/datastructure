模板类中：

- 类内函数声明中用到类名的地方不需要再加模板参数说明

- 类外定义中需要另加模板参数声明

  ```c++
  template<typename DataType,typename WeightType>
  class Digraph
  {
      public:
          Digraph();
  };
  template<typename DataType,typename WeightType>
  Digraph<DataType,WeightType>::Digraph(){}
  ```

  