from glob import glob
import os

from setuptools import setup

package_name = 'upf4ros2_demo'

setup(
    name=package_name,
    version='0.0.0',
    packages=[package_name],
    data_files=[
        ('share/ament_index/resource_index/packages',
            ['resource/' + package_name]),
        ('share/' + package_name, ['package.xml']),
        (os.path.join('share', package_name, 'launch'), glob('launch/*.launch.py')),
        (os.path.join('share', package_name, 'pddl'), glob('test/pddl/*')),
        (os.path.join("share", package_name, "params"), glob("params/*")),
    ],
    install_requires=['setuptools'],
    zip_safe=True,
    maintainer='gentlebots',
    maintainer_email='igonzf06@estudiantes.unileon.es',
    description='ROS 2 Support for UPF',
    license='Apache License, Version 2.0',
    tests_require=['pytest'],
    entry_points={
        'console_scripts': [
            'plan_executor = upf4ros2_demo.plan_executor:main',
            'navigation_action_client = upf4ros2_demo.navigation_action_client:main',
        ],
    },
)
