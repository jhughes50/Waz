"""
    Jason Hughes
    June 2024
    Object for semantics
"""
from typing import List, Tuple, Dict

class SemanticMappings:

    def __init__(self, results):
        self.map_ = None
        self.initialize_map(results)

    @property 
    def map(self) -> Dict[str, int]:
        return self.map_

    def initialize_map(self, results: List) -> Dict[str, int]:
        for i, d in enumerate(results):
            self.map_[d['label']] = i
