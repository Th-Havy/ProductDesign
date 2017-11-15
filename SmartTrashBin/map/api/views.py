from map.models import TrashBin, TrashBinState
from .serializers import TrashBinSerializer, TrashBinStateSerializer
from rest_framework.generics import ListCreateAPIView, RetrieveUpdateDestroyAPIView


class TrashBinList(ListCreateAPIView):
    """View to list all trash bins and add new ones"""
    queryset = TrashBin.objects.all()
    serializer_class = TrashBinSerializer


class TrashBinDetail(RetrieveUpdateDestroyAPIView):
    """View to get detail on a trash bin"""
    queryset = TrashBin.objects.all()
    serializer_class = TrashBinSerializer

class TrashBinStateList(ListCreateAPIView):
    serializer_class = TrashBinStateSerializer

    def get_queryset(self):
        """ Return the states of a particular trash bin"""
        trashId = self.kwargs['pk']
        return TrashBinState.objects.filter(trashBin__pk=trashId)

# Is this one necessary ? Maybe RetrieveAPIView is enough ?
#class TrashBinStateDetail(RetrieveUpdateDestroyAPIView):
#    queryset = TrashBinState.objects.all()
#    serializer_class = TrashBinStateSerializer
