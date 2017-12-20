* # Api_ws 说明:
api_ws是一个与安卓端交互的软件接口。
>Note 1：按照时间顺序来介绍使用的节点及其功能。

* ## step 1: 开机子启动 
来自/etc/rc.local 中指向主目录下面的minimal.sh, 而这个minimal.sh 指向 roslaunch operation bringup.launch

于是启动了两个节点：

* 节点1 是启动websocket的通信节点 (长连)
>roslaunch rosbridge_server rosbridge_websocket.launch

* 节点2 是用于地图建图的节点,但也是整个api的主节点，一旦mapping_status 中的3被调用，在结束的部分自动调用编辑地图部分的节点1. （长连接）
>roslaunch operation operation_mapbuild.launch

而这个节点中主要使用的是/mapping_status的service，用于建立地图，其中0,1,2,3，分别表示建图，暂存，保存并退出，和取消保存并退出。

这里使用了c++中的system函数调用shell脚本，进行地图的增删查改操作。这里假设服务器地址是：http://192.168.125.115，地图和相关文件保存在[服务器](http://192.168.125.115)上。其中map_lists.json 是保存的地图列表信息，所有的地图信息都会保存在这里，任何地图增删查改操作也直接作用于这里。

0:
>#!/bin/bash <br/>
echo "mapsaver bringup start."<br/>
roslaunch operation mapping.launch &</br>
sleep 10</br>

1:
>#!/bin/bash <br/>
echo "mapsaver pause."<br/>
. `rospack find operation`/src/sh_fun/system_op.sh<br/>
folder_init $1 $2 $3<br/>
sleep 0.5<br/>
copy_map $1 $2 $3<br/>
set_map_param $1 $2 $3 1<br/>
roslaunch map_server json_op.launch<br/>
echo "<<===========map saved to $1/$2/$2.pgm======================>>"

2:
>#! /bin/bash<br/>
#kill -9 `pgrep -f ros`<br/>
echo "mapsaver save, complete saving the map." &<br/>
#complete the mapping process.<br/>
. `rospack find operation`/src/sh_fun/system_op.sh<br/>
folder_init $1 $2 $3<br/>
copy_map $1 $2 $3<br/>
set_map_param $1 $2 $3 1<br/>
roslaunch map_server json_op.launch<br/>
stop_ros<br/>
echo map saved to $1/$2/$2.pgm<br/>

3:
>#!/bin/bash<br/>
#kill -9 `pgrep -f ros`<br/>
#stop all except rosbridge_websocket.<br/>
echo "mapsaver cancel."<br/>
. `rospack find operation`/src/sh_fun/system_op.sh -r $1/$2<br/>
#delete this map from map_lists.json<br/>
set_map_param $1 $2 $3 3<br/>
roslaunch map_server json_op.launch<br/>
stop_ros<br/>
#rosnode cleanup<br/>
echo "rosnode kill finished."<br/>


* ## step 2：编辑地图
在新建了地图之后，需要编辑地图。

这里需要启动的节点有：

* 节点1 地图编辑管理节点
>roslaunch tf_learning map_manager.launch

这里有4个service可以分别调用。作为服务端接受安卓端提供的service请求进行nav_pose,wall_pose,plan，task的增删查改。注意的地图frame和图片frame上的点的位置不同，我们对nav_pose保存为points下的points.json和graphpoints.json，对地图也是，保存为wall.json和graphwall.json，plan 保存为graphplan.json和plan.json.<br/>
同时，如果加载的地图map_id(最好用rosparam来设置)与之前的last_map_id不一致，将会调用新的map_server.launch.


* 节点2 地图的frame转换的节点，以及地图加载节点
> roslaunch map_server map_server.launch map_file:=?
> roslaunch tf_learning  picture_frame_pub.launch
