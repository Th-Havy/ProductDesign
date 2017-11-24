""" Contains serializers for the models

A serializer is a tool that converts data from the database into a readable or
transmittable format (JSON in this case).
"""

from rest_framework import serializers
from ..models import TrashBin, TrashBinState

class TrashBinStateSerializer(serializers.ModelSerializer):
    class Meta:
        model = TrashBinState
        fields = ('trashBin', 'fullness', 'time')

class TrashBinSerializer(serializers.ModelSerializer):
    state = TrashBinStateSerializer(required=False, allow_null=True)

    class Meta:
        model = TrashBin
        fields = ('pk', 'wasteType', 'latitude', 'longitude', 'floor', 'building', 'state')
