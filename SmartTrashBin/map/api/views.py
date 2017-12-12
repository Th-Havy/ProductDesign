from map.models import TrashBin, TrashBinState
from .serializers import TrashBinSerializer, TrashBinStateSerializer
from rest_framework.generics import ListCreateAPIView, RetrieveUpdateDestroyAPIView
from rest_framework import permissions
from datetime import datetime
from django.utils.dateparse import parse_datetime


class TrashBinList(ListCreateAPIView):
    """View to list all trash bins and add new ones"""
    queryset = TrashBin.objects.all()
    serializer_class = TrashBinSerializer
    permission_classes = (permissions.IsAuthenticatedOrReadOnly,)


class TrashBinDetail(RetrieveUpdateDestroyAPIView):
    """View to get detail on a trash bin"""
    queryset = TrashBin.objects.all()
    serializer_class = TrashBinSerializer
    permission_classes = (permissions.IsAuthenticatedOrReadOnly,)

class TrashBinStateList(ListCreateAPIView):
    """View to list the states of a trash bin and add new ones"""
    serializer_class = TrashBinStateSerializer
    permission_classes = (permissions.IsAuthenticatedOrReadOnly,)

    def get_queryset(self):
        """Return the states of a particular trash bin"""

        # Retrieve the trash bin states thanks to its ID (pk)
        trashId = self.kwargs['pk']
        queryset =  TrashBinState.objects.filter(trashBin__pk=trashId)

        # If a date is specified
        date = self.request.query_params.get('date', None)
        if date is not None:
            date = parse_datetime(date)
            queryset = queryset.filter(time__year=date.year, time__month=date.month, time__day=date.day)

        return queryset

    def perform_create(self, serializer):
        """Called when a new state is added"""
        # Make sure to add the state to the good trash bin
        trashBin=TrashBin.objects.get(pk=self.kwargs['pk'])
        state = serializer.save(trashBin=trashBin)

        # If the state time is more recent, update the trash bin state
        if trashBin.state.time <= state.time:
            trashBin.state = state
            trashBin.save()
