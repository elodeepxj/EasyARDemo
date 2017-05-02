# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Cmake的版本号,必填,如果不知道要怎么写,可以创建一个Demo 看看自己的Cmake版本号
cmake_minimum_required(VERSION 3.4.1)

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.

#设置Cmake的路径 "ProjectRoot" 是命名空间 方便后面的属性调用
set(ProjectRoot D:/AndroidStudioProject/EsayARDemo/app)

#这里负责创建主要的把没有编译的.CPP文件进行编译,加入你添加了一些cpp文件,那么你需要
#在这里添加进行编译.
# Searches for a specified prebuilt library and stores the path as a
# variable. Because system libraries are included in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build. 官方注释

add_library(
            # 设置需要编译出来的so名字,直接给java层调用的,不能有后缀名.so,也不需要
            myAR
            # 设置级别,这里可以SHARED STATIC 还有..具体看文档
            SHARED
            #下面的参数就是你要编译的cpp文件,例如
            src/main/cpp/ar.cc
            src/main/cpp/helloar.cc
            src/main/cpp/renderer.cc
            )


#这里的addlibrary是当你引用别人的so包的时候,需要实现一个命名空间,等下面的target调用,然后要加IMPORT标志
add_library(libEasyAR SHARED IMPORTED)

#寻找库文件,这里寻找的是android的log 不过详情我也没有去看
find_library(
              # Sets the name of the path variable.
              log-lib

              # Specifies the name of the NDK library that
              # you want CMake to locate.
              log
              android
              )

# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.

#这里的target官方表示是用于寻找你已经存在的so包,然后去依赖调用
set_target_properties(
                        # 制定你的目标,当你制定了目标 必须在add_library里面添加命名空间,才能知道你有这个lib
                       libEasyAR

                        #title的类型 制定你是import别人的so包
                        PROPERTIES IMPORTED_LOCATION

                        #导入你已经存在的静态so库,下面的绝对路径
                        ${ProjectRoot}/libs/armeabi-v7a/libEasyAR.so)


#这里表示你要引用的ndk包含的包的关键字
#例如说你要调用openGl 那么你要把OpenGl的关键字找出来
target_link_libraries(
                        # Specifies the target library.
                        #指定你的lib名字,也就是编译出来的so名字 和上面初始化的要对应
                         myAR

                         #指定你需要的调用的库 ,如果你有调用so包,那么也需要把so包的名字写进去
                         # Links the target library to the log library
                         # included in the NDK.
                         GLESv2
                         libEasyAR
                         log
                         ${log-lib} )