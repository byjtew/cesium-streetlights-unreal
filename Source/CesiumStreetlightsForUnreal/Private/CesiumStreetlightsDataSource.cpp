#include "CesiumStreetlightsDataSource.h"


void UCesiumStreetlightsDataSource::AsyncRequest(ENamedThreads::Type thread_name, FCesiumStreetlightsRequestConfiguration configuration, TFunction<void(FCesiumStreetlightsRequestResult&&)> on_results)
{
	AsyncTask(
		thread_name,
		[self=this, configuration, on_results]
		{
			TRACE_CPUPROFILER_EVENT_SCOPE(UCesiumStreetlightsGeodeskDataSource::AsyncRequest);
			self->RequestInternal(configuration, on_results);
		}
	);
}

void UCesiumStreetlightsDataSource::Request(FCesiumStreetlightsRequestConfiguration configuration, TFunction<void(FCesiumStreetlightsRequestResult&&)> on_results)
{
	TRACE_CPUPROFILER_EVENT_SCOPE(UCesiumStreetlightsGeodeskDataSource::Request);
	RequestInternal(configuration, on_results);
}

void UCesiumStreetlightsDataSource::RequestInternal(FCesiumStreetlightsRequestConfiguration configuration, TFunction<void(FCesiumStreetlightsRequestResult&&)> on_results)
{
	on_results({});
}
