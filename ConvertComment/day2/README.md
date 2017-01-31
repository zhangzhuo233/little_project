#ConvertComment_Test</b>
**The project has two main option**</b>
Usage: ./convert [-Option] src dest</b>
Option:	</b>
      -s,--simple		      转换单一文件
      </b>-R, -r --recursive  递归转换目录及其子目录内的所有内容	</b>
      -h  			          帮助信息</b>
</b>Version:			v1.0</b>
</b>Author:			zhangzhuo </b>
#几点说明：
-1.关于换行符
-Windows等操作系统用的文本换行符和UNIX/Linux操作系统用的不同，
-Windows系统下输入的换行符在UNIX/Linux下不会显示为“换行”，
而是显示为 ^M 这个符号（这是Linux等系统下规定的特殊标记，占一个字符大小，不是 ^ 和 M 的组合，打印不出来的）。
-Linux下很多文本编辑器（命令行）会在显示这个标记之后，补上一个自己的换行符，以避免内容混乱
（只是用于显示，补充的换行符不会写入文件，有专门的命令将Windows换行符替换为Linux换行符）。
-2.Linux下删除windows换行符^M
-单个的文件转换
-sed -i 's/\r//'  filename
-批量的文件转换
-sed -i 's/\r//'  filename1 filename2 ...
-或
-find conf/  -name "*.*" |xargs sed -i 's/\r//'
