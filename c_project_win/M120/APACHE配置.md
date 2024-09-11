# APACHE基本配置
## 基本语法
&emsp;&emsp;APACHE采用的文本配置，只需取消配置文件中语句前的 `#` 即可以添加配置。
## 全局配置
&emsp;&emsp;1.ServerRoot：服务器根目录，也就是Apache的安装目录，其他的目录配置若是使用相对路径，都是相对于服务器根目录的。配置示例：`ServerRoot "/usr/local/apache2"`
&emsp;&emsp;2.DocumentRoot：网站的根目录，也就是放置网站文件的地方。配置示例：`DocumentRoot = "/usr/local/apache2/htdocs"`  
&emsp;&emsp;3.Listen：服务器监听的端口，可以只配置端口，也可以同时指定IP地址和端口，还可以指定使用的协议（可配置多个端口）。配置示例：`Listen 127.0.0.1:80 --ipv4` or `Listen [2001:db8::a00:20ff:fea7:ccea]:80 --ipv6`
&emsp;&emsp;4.ErrorLog：指定当服务器遇到错误时记录错误日志的文件（若不是一个以“/”开头的绝对路径，则是一个相对于ServerRoot的相对路径）。配置示例：`ErrorLog "logs/error.log"`  
&emsp;&emsp;5.LogLevel：用于调整记录在错误日志中的信息的详细程度。配置示例：`LogLevel  warn`。 错误信息详细程度按重要性降序排列如下：

        emerg：紧急（系统无法使用）
        alert：必须立即采取措施。
        crit：致命情况。
        error：错误情况。
        warn：警告情况。
        notice：一般重要情况。
        info：普通信息。
        debug：调试信息。

&emsp;&emsp;7.User：设置实际提供服务的子进程的用户。配置示例：`User daemon`  
&emsp;&emsp;8.Group：设置提供服务的Apache子进程运行时的用户组。配置示例：`Group  daemon`
## Module配置
&emsp;&emsp;`LoadModule`：加载特定的DSO模块，这些已编译的DSO模块存放于Apache安装目录下的`modules`目录中。只有加载正确的`LoadModule`才能使对应的模块配置在启动APACHE服务器时生效。

        LoadModule dir_module modules/mod_dir.so
        #
        # DirectoryIndex: sets the file that Apache will serve if a directory
        # is requested.
        #
        <IfModule dir_module>
             DirectoryIndex index.html
        </IfModule>

## 目录配置
&emsp;&emsp;`<Directory>`和`</Directory>`一起用于封装一组指令，使之仅对某个目录及其子目录生效。该指令不能被嵌套使用，也不能出现在`<Limit>`或`<LimitExcept>`配置段中。

        <Directory "/usr/local/apache2/cgi-bin">
            AllowOverride None  #不允许.htaccess 文件中设定权限覆盖
            Options None #不启用任何服务器特性
            Require all granted #允许所有请求
        </Directory>

## HTTPS配置
&emsp;&emsp;SSL配置至少需要包含遵循指令。

        LoadModule ssl_module modules/mod_ssl.so

        Listen 443
        <VirtualHost *:443>
            ServerName www.example.com
            SSLEngine on
            SSLCertificateFile "/path/to/www.example.com.cert"
            SSLCertificateKeyFile "/path/to/www.example.com.key"
        </VirtualHost>

&emsp;&emsp;SSL 配置的自签名的证书。配置示例：`SSLCertificateFile "conf/server.crt"`   
&emsp;&emsp;SSL 配置的自签名的私钥。配置示例：`SSLCertificateKeyFile "conf/server.key"` 
&emsp;&emsp;SSL 配置的可用的协议版本。配置示例：`SSLProtocol all -SSLv3 -TLSv1 -TLSv1.1`
&emsp;&emsp;为代理使用配置可用的 SSL 协议版本。配置示例：`SSLProxyProtocol all -SSLv3 -TLSv1 -TLSv1.1`
