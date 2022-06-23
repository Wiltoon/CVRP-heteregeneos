# criar os types necessarios para passar o kmeans
import json
from dataclasses import dataclass, asdict
from pathlib import Path
from typing import List, Union

from dacite import from_dict

class JSONDataclassMixin:
    """Mixin for adding JSON file capabilities to Python dataclasses."""
    @classmethod
    def from_file(cls, path: Union[Path, str]) -> "JSONDataclassMixin":
        """Load dataclass instance from provided file path."""

        with open(path) as f:
            data = json.load(f)

        return from_dict(cls, data)

    def to_file(self, path: Union[Path, str]) -> None:
        """Save dataclass instance to provided file path."""

        with open(path, "w") as f:
            json.dump(asdict(self), f)

        return

@dataclass
class ParamsVehicles(JSONDataclassMixin):
    types: List[str]
    """Tipo de veículos existentes."""

    num_types: List[float]
    """Número de tipos de veículo que deve existir no problema."""

    capacities: List[int]
    """Capacidade dos veículos existentes."""

    custs: List[int]
    """Custos dos veículos existentes."""