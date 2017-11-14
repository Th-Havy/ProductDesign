""" Contains serializers for the models

A serializer is a tool that converts data from the database into a readable or
transmittable format (JSON in this case).
"""

from rest_framework import serializers
from ..models import TrashBin, TrashBinState

class TrashBinSerializer(serializers.ModelSerializer):
    class Meta:
        model = TrashBin
        fields = ('pk', 'latitude', 'longitude', 'floor', 'building', 'state')
        #read_only_fields = ('date_created', 'date_modified')

class TrashBinStateSerializer(serializers.ModelSerializer):
    class Meta:
        model = TrashBinState
        fields = ('fullness', 'time')
