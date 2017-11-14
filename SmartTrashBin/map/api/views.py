from map.models import TrashBin, TrashBinState
from .serializers import TrashBinSerializer, TrashBinStateSerializer
from rest_framework.generics import ListCreateAPIView, RetrieveUpdateDestroyAPIView


class TrashBinList(ListCreateAPIView):
    queryset = TrashBin.objects.all()
    serializer_class = TrashBinSerializer


class TrashBinListDetail(RetrieveUpdateDestroyAPIView):
    queryset = TrashBin.objects.all()
    serializer_class = TrashBinSerializer

class TrashBinStateList(ListCreateAPIView):
    queryset = TrashBinState.objects.all()
    serializer_class = TrashBinStateSerializer


class TrashBinStateListDetail(RetrieveUpdateDestroyAPIView):
    queryset = TrashBinState.objects.all()
    serializer_class = TrashBinStateSerializer
