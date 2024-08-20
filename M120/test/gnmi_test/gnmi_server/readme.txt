1，需要jsoncpp库，安装步骤如下：
   --git clone https://github.com/open-source-parsers/jsoncpp.git
   --cd jsoncpp; & mkdir build & cd build;
   --cmake ..
   --cd lib/
   --sudo cp -rf * /usr/local/lib/
   --cd ../../include/
   --sudo mkdir -p /usr/local/include/jsoncpp
   --sudo mv json /usr/local/include/jsoncpp/