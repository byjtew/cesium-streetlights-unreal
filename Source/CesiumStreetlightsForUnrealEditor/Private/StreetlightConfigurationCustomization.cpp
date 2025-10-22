#include "StreetlightConfigurationCustomization.h"

#include "StreetlightStyleConfiguration.h"
#include "DetailWidgetRow.h"
#include "IDetailChildrenBuilder.h"

TSharedRef<IPropertyTypeCustomization> FStreetlightConfigurationCustomization::MakeInstance()
{
	return MakeShared<FStreetlightConfigurationCustomization>();
}

void FStreetlightConfigurationCustomization::CustomizeHeader(
	TSharedRef<IPropertyHandle>      PropertyHandle,
	FDetailWidgetRow&                HeaderRow,
	IPropertyTypeCustomizationUtils& CustomizationUtils
)
{
	FStreetlightStyleConfiguration* configuration = nullptr;

	// Get the raw struct data
	{
		void* raw_data = nullptr;
		if (PropertyHandle->GetValueData(raw_data) == FPropertyAccess::Success && raw_data)
		{
			configuration = static_cast<FStreetlightStyleConfiguration*>(raw_data);
		}
	}

	if (configuration)
	{
		HeaderRow
			.NameContent()
			[
				PropertyHandle->CreatePropertyNameWidget()
			]
			.ValueContent()
			[
				SNew(SVerticalBox)
				+ SVerticalBox::Slot().AutoHeight()
				[
					SNew(SButton)
					.Text(FText::FromString("Preset: Default"))
					.OnClicked_Lambda([=]
					{
						configuration->Preset_Default();
						return FReply::Handled();
					}),
					SNew(SButton)
					.Text(FText::FromString("Preset: Residential"))
					.OnClicked_Lambda([=]
					{
						configuration->Preset_Residential();
						return FReply::Handled();
					}),
					SNew(SButton)
					.Text(FText::FromString("Preset: Alley"))
					.OnClicked_Lambda([=]
					{
						configuration->Preset_Alley();
						return FReply::Handled();
					}),
					SNew(SButton)
					.Text(FText::FromString("Preset: Motorway"))
					.OnClicked_Lambda([=]
					{
						configuration->Preset_Motorway();
						return FReply::Handled();
					}),
					SNew(SButton)
					.Text(FText::FromString("Preset: Railway"))
					.OnClicked_Lambda([=]
					{
						configuration->Preset_Railway();
						return FReply::Handled();
					})
				]
			];
	}
}

void FStreetlightConfigurationCustomization::CustomizeChildren(
	TSharedRef<IPropertyHandle>      PropertyHandle,
	IDetailChildrenBuilder&          ChildBuilder,
	IPropertyTypeCustomizationUtils& CustomizationUtils
)
{
	// Then add struct’s normal fields
	uint32 NumChildren;
	PropertyHandle->GetNumChildren(NumChildren);
	for (uint32 i = 0; i < NumChildren; ++i)
	{
		ChildBuilder.AddProperty(PropertyHandle->GetChildHandle(i).ToSharedRef());
	}
}
