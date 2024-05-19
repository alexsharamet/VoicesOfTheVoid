from conan import ConanFile
from conan.tools.cmake import cmake

class AI_SERVER(ConanFile):
    name = "ai_server"
    version = "1.0"
    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeToolchain", "CMakeDeps"
    requires = [
        ("openssl/3.2.0"),
        ("gtest/cci.20210126"),
        ("libcurl/8.5.0"),
        ("nlohmann_json/3.11.3")
    ]
    default_options = {
        "libcurl/*:with_ssl": "openssl",
    }
