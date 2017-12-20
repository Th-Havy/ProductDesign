from django.contrib import admin
from django.utils.translation import ugettext_lazy
from .models import TrashBin, TrashBinState

admin.site.register(TrashBin)
admin.site.register(TrashBinState)

# Customize admin site
# <title>
admin.site.site_title = ugettext_lazy('Smart Trash Bin')
# <h1> (and above login form)
admin.site.site_header = ugettext_lazy('Smart Trash Bin Administration')
# top of the admin index page
admin.site.index_title = ugettext_lazy('Smart Trash Bin Administration')
