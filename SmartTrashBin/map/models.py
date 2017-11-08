###

from django.contrib.gis.db import models

class TrashBin(models.Model):
    # Location (latitude, longitude, floor)
    location = models.PointField(dim=3)

    # Picture ?

    # Current state
    def state(self):
        # get list of state and return the latest
        pass

class TrashBinState:
    # Fullness of a trash bin (0=empty, 100=completely full)
    fullness = models.FloatField(default=0)

    # When was this state recorded
    time = models.DateTimeField()

    # The trash bin this instance is the state of
    trashBin = models.ForeignKey(TrashBin, on_delete=models.CASCADE, related_name=state_history)
