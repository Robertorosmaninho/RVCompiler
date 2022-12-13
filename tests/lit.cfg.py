import lit.formats
import os

config.name = "RV Compiler Tests"
config.test_format = lit.formats.ShTest(True)

config.suffixes = ['.cpp']

config.test_source_root = os.path.dirname(__file__)
config.test_exec_root = os.path.join(config.my_obj_root, 'tests')

config.substitutions.append(('%RVCompiler',
    os.path.join(config.my_obj_root, 'bin/RVCompiler')))

config.substitutions.append(('%inputs',
    os.path.join(config.test_source_root, 'inputs')))
