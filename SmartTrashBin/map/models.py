"""Contains the definition of the models.

A model is a python representation of data stored in the database."""
from django.contrib.gis.db import models

class TrashBin(models.Model):
    """Model of a trash bin."""
    # Location (latitude, longitude, floor)
    location = models.PointField(dim=3)

    # Picture ?

    # Current state
    def state(self):
        # get list of state and return the latest
        pass

class TrashBinState:
    """Fullness state of a given trash bin."""
    # Fullness of a trash bin (0=empty, 100=completely full)
    fullness = models.FloatField(default=0)

    # When was this state recorded
    time = models.DateTimeField()

    # The trash bin this instance is the state of
    trashBin = models.ForeignKey(TrashBin, on_delete=models.CASCADE, related_name=state_history)
