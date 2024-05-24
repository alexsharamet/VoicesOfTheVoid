from conan import ConanFile
from conan.tools.cmake import cmake

class AI_Client(ConanFile):
    name = "ai_client"
    version = "1.0"
    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeToolchain", "CMakeDeps"
    requires = [
        ("nlohmann_json/3.11.3"),
        ("cpp-httplib/0.15.3"),
        ("fmt/10.2.1")
    ]
