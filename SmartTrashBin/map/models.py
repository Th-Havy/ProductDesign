"""Contains the definition of the models.

A model is a python representation of data stored in the database."""
from django.db import models

class TrashBin(models.Model):
    """Model of a trash bin."""
    # Location
    latitude = models.FloatField()
    longitude = models.FloatField()
    floor = models.IntegerField()
    building = models.CharField(max_length=50)

    # Most recent state
    state = models.OneToOneField('TrashBinState', on_delete=models.SET_NULL,
                                 related_name='+')

class TrashBinState:
    """Fullness state of a given trash bin."""
    # Fullness of a trash bin (0=empty, 100=completely full)
    fullness = models.FloatField(default=0)

    # When was this state recorded
    time = models.DateTimeField()

    # The trash bin this instance is the state of
    trashBin = models.ForeignKey(TrashBin, on_delete=models.CASCADE,
                                 related_name='state_history')
