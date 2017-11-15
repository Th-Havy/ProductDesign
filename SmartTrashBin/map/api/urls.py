from django.conf.urls import url
from rest_framework.urlpatterns import format_suffix_patterns
from . import views

# Namespace for the urls ('api:url_name')
app_name = 'api'

urlpatterns = [
    # website.com/api/trashbin/
    url(r'^trashbin/$', views.TrashBinList.as_view(), name='trashbin-list'),
    # website.com/api/trashbin/12/
    url(r'^trashbin/(?P<pk>[0-9]+)/$', views.TrashBinDetail.as_view(), name='trashbin-detail'),
    # website.com/api/trashbin/12/state/
    url(r'^trashbin/(?P<pk>[0-9]+)/state/$', views.TrashBinStateList.as_view(), name='trashbin-state'),
]

urlpatterns = format_suffix_patterns(urlpatterns)
