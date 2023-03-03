from conans import ConanFile, CMake, tools


class MatrixlibConan(ConanFile):
    name = "matrixlib"
    version = "0.0.1"
    settings = "os", "compiler", "build_type", "arch"
    options = {
        "shared": [True, False],
        "fPIC": [True, False]
    }
    default_options = {
        "shared": False,
        "fPIC": True
    }
    generators = "cmake_find_package_multi"

    def source(self):
        git = tools.Git(folder="matrixlib")
        git.clone("https://github.com/Pyshich/matrixlib")

    def requirements(self):
        self.requires("fmt/9.1.0")

    def config_options(self):
        if self.settings.os == "Windows":
            del self.options.fPIC

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package(self):
        self.copy("*.h", dst="include", src="matrixes/include")
        self.copy("*.dll", dst="bin", keep_path=False)
        self.copy("*.so", dst="lib", keep_path=False)
        self.copy("*.dylib", dst="lib", keep_path=False)
        self.copy("*.a", dst="lib", keep_path=False)

    def package_info(self):
        self.cpp_info.libs = ["matrixlib"]
