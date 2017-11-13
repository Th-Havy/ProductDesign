from rest_framework import serializers
from .models import TrashBin, TrashBinState

class TrashBinSerializer(serializers.ModelSerializer):
    class Meta:
        model = TrashBin

class TrashBinStateSerializer(serializers.ModelSerializer):
    class Meta:
        model = TrashBinState
