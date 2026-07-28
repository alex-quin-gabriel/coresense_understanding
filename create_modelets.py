#! /usr/bin/env python
from os.path import splitext, split, join
from jinja2 import Environment, PackageLoader, select_autoescape, FileSystemLoader

env = Environment(
    loader=FileSystemLoader("/home/alex/plansys2_ws/src/coresense_modelet_interfaces"),
    autoescape=select_autoescape()
)

env.trim_blocks = True
#env.lstrip_blocks = True

modelets = {
        'modelets': [
            {
                'name': 'range',
                'name_camel': 'Range',
                'package': 'sensor_msgs',
                'concepts': [
                    'first', 'second'
                ],
                'representation_classes': [
                    'first', 'second'
                ],
                'properties': [
                    'first', 'second'
                ],
                'fields': [
                    {
                        'name': 'radiation_type',
                        'type': 'int'
                    },
                    {
                        'name': 'field_of_view',
                        'type': 'float'
                    }, 
                    {
                        'name': 'min_range',
                        'type': 'float'
                    }, 
                    {
                        'name': 'max_range',
                        'type': 'float'
                    }, 
                    {
                        'name': 'range',
                        'type': 'float'
                    } 
                ]
            }
        ]
    }


global_files = ['package.xml.jinja', 'CMakeLists.txt.jinja']

per_modelet_files = ['config/get_modelet.json.jinja', 'behavior_trees/get_modelet.xml.jinja', 'include/coresense_modelet_interfaces/get_modelet.hpp.jinja', 'src/get_modelet.cpp.jinja']
if __name__ == '__main__':
    for file in global_files:
        template = env.get_template(file)
        with open(file.rpartition('.jinja')[0], 'w') as f:
            f.write(template.render(modelets))
    for modelet in modelets['modelets']:
        for file in per_modelet_files:
            template = env.get_template(file)
            path, template_name = split(file)
            file_name = splitext(template_name)[0][4:]
            target_file = join(path, 'get_' + modelet['name'] + '_' + file_name)

            with open(target_file, 'w') as f:
                f.write(template.render(modelet))


