from conans import ConanFile
from conan.tools.scm import Git
from conan.tools.cmake import CMake, CMakeToolchain


class MatrixlibConan(ConanFile):
    name = "matrixlib"
    desciption = "MatrixLib - an easy matrix library"
    url = "https://github.com/Psyhich/matrixlib"
    license = "WTFPL"

    version = "0.0.3"
    settings = "os", "compiler", "build_type", "arch"
    options = {
        "shared": [True, False],
        "fPIC": [True, False],
        "build_tests": [True, False]
    }
    default_options = {
        "shared": False,
        "fPIC": True,
        "build_tests": False
    }
    generators = "CMakeDeps"

    def config_options(self):
        if self.settings.os == "Windows":
            del self.options.fPIC

    def source(self):
        git = Git(self)
        git.clone("https://github.com/Psyhich/matrixlib.git", target=".")
        git.checkout("conan_update")

    def requirements(self):
        self.requires("fmt/9.1.0")
        if self.options.build_tests:
            self.requires("gtest/1.12.1")

    def generate(self):
        tc = CMakeToolchain(self)
        tc.cache_variables["ENABLE_TESTING"] = self.options.build_tests
        tc.cache_variables["ENABLE_DEVELOPER_MODE"] = False
        tc.generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package(self):
        self.copy("*.h", dst="matrixlib/", src="matrixes/include")
        self.copy("*.dll", dst="bin", keep_path=False)
        self.copy("*.so", dst="lib", keep_path=False)
        self.copy("*.dylib", dst="lib", keep_path=False)
        self.copy("*.a", dst="lib", keep_path=False)

    def package_info(self):
        self.cpp_info.libs = ["matrixlib"]
        self.cpp_info.include = ["matrixlib"]
