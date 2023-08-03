#pragma once

namespace melatonin
{
    class Overlay;

    class ComponentTreeViewItem
        : public juce::TreeViewItem,
          public juce::MouseListener,
          public juce::ComponentListener
    {
    public:
        bool hasTabbedComponent = false;

        explicit ComponentTreeViewItem (juce::Component* c,
            std::function<void (juce::Component* c)> outline,
            std::function<void (juce::Component* c)> select)
            : outlineComponentCallback (outline), selectComponentCallback (select), component (c)
        {
            // A few JUCE component types need massaging to get their child components
            if (auto multiPanel = dynamic_cast<juce::MultiDocumentPanel*> (c))
            {
                recursivelyAddChildrenFor (multiPanel->getCurrentTabbedComponent());
            }
            else if (auto tabs = dynamic_cast<juce::TabbedComponent*> (c))
            {
                hasTabbedComponent = true;
                for (int i = 0; i < tabs->getNumTabs(); ++i)
                {
                    recursivelyAddChildrenFor (tabs->getTabContentComponent (i));
                }
            }
            else
            {
                addItemsForChildComponents();
            }

            setDrawsInLeftMargin (true);

            // Make our tree self-aware
            component->addComponentListener (this);
        }

        ~ComponentTreeViewItem() override
        {
            // The component can be deleted before this tree view item
            if (component)
                component->removeComponentListener (this);
        }

        juce::Path getKeyboardIcon()
        {
            static juce::Path p = []
            {
                static const unsigned char pathData[] = { 110,109,0,0,128,66,0,0,128,66,98,154,153,229,65,0,0,128,66,0,0,0,0,102,102,185,66,0,0,0,0,0,0,0,67,108,0,0,0,0,0,0,192,67,98,0,0,0,0,102,166,209,67,154,153,229,65,0,0,224,67,0,0,128,66,0,0,224,67,108,0,0,0,68,0,0,224,67,98,51,211,8,68,0,0,224,67,0,0,
                    16,68,102,166,209,67,0,0,16,68,0,0,192,67,108,0,0,16,68,0,0,0,67,98,0,0,16,68,102,102,185,66,51,211,8,68,0,0,128,66,0,0,0,68,0,0,128,66,108,0,0,128,66,0,0,128,66,99,109,0,0,160,66,0,0,0,67,108,0,0,224,66,0,0,0,67,98,154,153,241,66,0,0,0,67,0,0,0,67,51,
                    51,7,67,0,0,0,67,0,0,16,67,108,0,0,0,67,0,0,48,67,98,0,0,0,67,205,204,56,67,154,153,241,66,0,0,64,67,0,0,224,66,0,0,64,67,108,0,0,160,66,0,0,64,67,98,102,102,142,66,0,0,64,67,0,0,128,66,205,204,56,67,0,0,128,66,0,0,48,67,108,0,0,128,66,0,0,16,67,98,0,
                    0,128,66,51,51,7,67,102,102,142,66,0,0,0,67,0,0,160,66,0,0,0,67,99,109,0,0,128,66,0,0,112,67,98,0,0,128,66,51,51,103,67,102,102,142,66,0,0,96,67,0,0,160,66,0,0,96,67,108,0,0,224,66,0,0,96,67,98,154,153,241,66,0,0,96,67,0,0,0,67,51,51,103,67,0,0,0,67,
                    0,0,112,67,108,0,0,0,67,0,0,136,67,98,0,0,0,67,102,102,140,67,154,153,241,66,0,0,144,67,0,0,224,66,0,0,144,67,108,0,0,160,66,0,0,144,67,98,102,102,142,66,0,0,144,67,0,0,128,66,102,102,140,67,0,0,128,66,0,0,136,67,108,0,0,128,66,0,0,112,67,99,109,0,0,
                    160,66,0,0,160,67,108,0,0,224,66,0,0,160,67,98,154,153,241,66,0,0,160,67,0,0,0,67,154,153,163,67,0,0,0,67,0,0,168,67,108,0,0,0,67,0,0,184,67,98,0,0,0,67,102,102,188,67,154,153,241,66,0,0,192,67,0,0,224,66,0,0,192,67,108,0,0,160,66,0,0,192,67,98,102,102,
                    142,66,0,0,192,67,0,0,128,66,102,102,188,67,0,0,128,66,0,0,184,67,108,0,0,128,66,0,0,168,67,98,0,0,128,66,154,153,163,67,102,102,142,66,0,0,160,67,0,0,160,66,0,0,160,67,99,109,0,0,32,67,0,0,16,67,98,0,0,32,67,51,51,7,67,51,51,39,67,0,0,0,67,0,0,48,67,
                    0,0,0,67,108,0,0,80,67,0,0,0,67,98,205,204,88,67,0,0,0,67,0,0,96,67,51,51,7,67,0,0,96,67,0,0,16,67,108,0,0,96,67,0,0,48,67,98,0,0,96,67,205,204,56,67,205,204,88,67,0,0,64,67,0,0,80,67,0,0,64,67,108,0,0,48,67,0,0,64,67,98,51,51,39,67,0,0,64,67,0,0,32,
                    67,205,204,56,67,0,0,32,67,0,0,48,67,108,0,0,32,67,0,0,16,67,99,109,0,0,48,67,0,0,96,67,108,0,0,80,67,0,0,96,67,98,205,204,88,67,0,0,96,67,0,0,96,67,51,51,103,67,0,0,96,67,0,0,112,67,108,0,0,96,67,0,0,136,67,98,0,0,96,67,102,102,140,67,205,204,88,67,
                    0,0,144,67,0,0,80,67,0,0,144,67,108,0,0,48,67,0,0,144,67,98,51,51,39,67,0,0,144,67,0,0,32,67,102,102,140,67,0,0,32,67,0,0,136,67,108,0,0,32,67,0,0,112,67,98,0,0,32,67,51,51,103,67,51,51,39,67,0,0,96,67,0,0,48,67,0,0,96,67,99,109,0,0,32,67,0,0,168,67,
                    98,0,0,32,67,154,153,163,67,51,51,39,67,0,0,160,67,0,0,48,67,0,0,160,67,108,0,0,200,67,0,0,160,67,98,102,102,204,67,0,0,160,67,0,0,208,67,154,153,163,67,0,0,208,67,0,0,168,67,108,0,0,208,67,0,0,184,67,98,0,0,208,67,102,102,188,67,102,102,204,67,0,0,192,
                    67,0,0,200,67,0,0,192,67,108,0,0,48,67,0,0,192,67,98,51,51,39,67,0,0,192,67,0,0,32,67,102,102,188,67,0,0,32,67,0,0,184,67,108,0,0,32,67,0,0,168,67,99,109,0,0,136,67,0,0,0,67,108,0,0,152,67,0,0,0,67,98,102,102,156,67,0,0,0,67,0,0,160,67,51,51,7,67,0,0,
                    160,67,0,0,16,67,108,0,0,160,67,0,0,48,67,98,0,0,160,67,205,204,56,67,102,102,156,67,0,0,64,67,0,0,152,67,0,0,64,67,108,0,0,136,67,0,0,64,67,98,154,153,131,67,0,0,64,67,0,0,128,67,205,204,56,67,0,0,128,67,0,0,48,67,108,0,0,128,67,0,0,16,67,98,0,0,128,
                    67,51,51,7,67,154,153,131,67,0,0,0,67,0,0,136,67,0,0,0,67,99,109,0,0,128,67,0,0,112,67,98,0,0,128,67,51,51,103,67,154,153,131,67,0,0,96,67,0,0,136,67,0,0,96,67,108,0,0,152,67,0,0,96,67,98,102,102,156,67,0,0,96,67,0,0,160,67,51,51,103,67,0,0,160,67,0,
                    0,112,67,108,0,0,160,67,0,0,136,67,98,0,0,160,67,102,102,140,67,102,102,156,67,0,0,144,67,0,0,152,67,0,0,144,67,108,0,0,136,67,0,0,144,67,98,154,153,131,67,0,0,144,67,0,0,128,67,102,102,140,67,0,0,128,67,0,0,136,67,108,0,0,128,67,0,0,112,67,99,109,0,
                    0,184,67,0,0,0,67,108,0,0,200,67,0,0,0,67,98,102,102,204,67,0,0,0,67,0,0,208,67,51,51,7,67,0,0,208,67,0,0,16,67,108,0,0,208,67,0,0,48,67,98,0,0,208,67,205,204,56,67,102,102,204,67,0,0,64,67,0,0,200,67,0,0,64,67,108,0,0,184,67,0,0,64,67,98,154,153,179,
                    67,0,0,64,67,0,0,176,67,205,204,56,67,0,0,176,67,0,0,48,67,108,0,0,176,67,0,0,16,67,98,0,0,176,67,51,51,7,67,154,153,179,67,0,0,0,67,0,0,184,67,0,0,0,67,99,109,0,0,176,67,0,0,112,67,98,0,0,176,67,51,51,103,67,154,153,179,67,0,0,96,67,0,0,184,67,0,0,96,
                    67,108,0,0,200,67,0,0,96,67,98,102,102,204,67,0,0,96,67,0,0,208,67,51,51,103,67,0,0,208,67,0,0,112,67,108,0,0,208,67,0,0,136,67,98,0,0,208,67,102,102,140,67,102,102,204,67,0,0,144,67,0,0,200,67,0,0,144,67,108,0,0,184,67,0,0,144,67,98,154,153,179,67,0,
                    0,144,67,0,0,176,67,102,102,140,67,0,0,176,67,0,0,136,67,108,0,0,176,67,0,0,112,67,99,109,0,0,232,67,0,0,0,67,108,0,0,248,67,0,0,0,67,98,102,102,252,67,0,0,0,67,0,0,0,68,51,51,7,67,0,0,0,68,0,0,16,67,108,0,0,0,68,0,0,48,67,98,0,0,0,68,205,204,56,67,102,
                    102,252,67,0,0,64,67,0,0,248,67,0,0,64,67,108,0,0,232,67,0,0,64,67,98,154,153,227,67,0,0,64,67,0,0,224,67,205,204,56,67,0,0,224,67,0,0,48,67,108,0,0,224,67,0,0,16,67,98,0,0,224,67,51,51,7,67,154,153,227,67,0,0,0,67,0,0,232,67,0,0,0,67,99,109,0,0,224,
                    67,0,0,112,67,98,0,0,224,67,51,51,103,67,154,153,227,67,0,0,96,67,0,0,232,67,0,0,96,67,108,0,0,248,67,0,0,96,67,98,102,102,252,67,0,0,96,67,0,0,0,68,51,51,103,67,0,0,0,68,0,0,112,67,108,0,0,0,68,0,0,136,67,98,0,0,0,68,102,102,140,67,102,102,252,67,0,
                    0,144,67,0,0,248,67,0,0,144,67,108,0,0,232,67,0,0,144,67,98,154,153,227,67,0,0,144,67,0,0,224,67,102,102,140,67,0,0,224,67,0,0,136,67,108,0,0,224,67,0,0,112,67,99,109,0,0,232,67,0,0,160,67,108,0,0,248,67,0,0,160,67,98,102,102,252,67,0,0,160,67,0,0,0,
                    68,154,153,163,67,0,0,0,68,0,0,168,67,108,0,0,0,68,0,0,184,67,98,0,0,0,68,102,102,188,67,102,102,252,67,0,0,192,67,0,0,248,67,0,0,192,67,108,0,0,232,67,0,0,192,67,98,154,153,227,67,0,0,192,67,0,0,224,67,102,102,188,67,0,0,224,67,0,0,184,67,108,0,0,224,
                    67,0,0,168,67,98,0,0,224,67,154,153,163,67,154,153,227,67,0,0,160,67,0,0,232,67,0,0,160,67,99,101,0,0 };

                juce::Path path;
                path.loadPathFromData (pathData, sizeof (pathData));
                return path;
            }();

            return p;
        }

        bool mightContainSubItems() override
        {
            return component != nullptr && (component->getNumChildComponents() > 0);
        }

        // naive but functional...
        void openTreeAndSelect (juce::Component* target)
        {
            // don't let us select something already selected
            if (component == target && !isSelected())
            {
                setSelected (true, true, juce::dontSendNotification);
                setOpen (true);
            }
            else if (component->isParentOf (target))
            {
                jassert (target);
                setOpen (true);
                // recursively open up tree to get at target
                for (int i = 0; i < getNumSubItems(); ++i)
                {
                    dynamic_cast<ComponentTreeViewItem*> (getSubItem (i))->openTreeAndSelect (target);
                }
            }
        }

        void paintItem (juce::Graphics& g, int w, int h) override
        {
            if (! component)
                return;

            auto textIndent = mightContainSubItems() ? 7 : 5;

            g.setColour (colors::treeItemText);

            if (isSelected())
            {
                g.setColour (colors::treeItemSelection);
                g.fillRect (-18 - textIndent, 0, w, h);
                g.setColour(colors::treeItemTextSelected);
            }

            if (! component->isVisible())
                g.setColour (colors::treeItemTextDisabled);

            auto name = componentString (component);
            auto font = juce::Font ("Verdana", 15, juce::Font::FontStyleFlags::plain);

            g.setFont (font);

            if (component->hasKeyboardFocus (false))
            {
                auto rc = juce::Rectangle<float> (textIndent - 4.0f, 6.0f, h - 12.0f, h - 12.0f);

                auto p = getKeyboardIcon();
                g.fillPath (p, p.getTransformToScaleToFit (rc, true));
                textIndent += int (rc.getWidth());
            }

            g.drawText (name, textIndent, 0, w - textIndent, h, juce::Justification::left, true);
        }

        // must override to set the disclosure triangle color
        void paintOpenCloseButton (juce::Graphics& g, const juce::Rectangle<float>& area, juce::Colour /*backgroundColour*/, bool isMouseOver) override
        {
            getOwnerView()->getLookAndFeel().drawTreeviewPlusMinusBox (g, area, colors::treeViewMinusPlusColor, isOpen(), isMouseOver);
        }

        void itemClicked (const juce::MouseEvent&) override
        {
            selectComponentCallback (component);
            selectTabbedComponentChildIfNeeded();
            if (mightContainSubItems())
                setOpen (true);
        }

        void mouseEnter (const juce::MouseEvent& /*event*/) override
        {
            outlineComponentCallback (component);
        }

        void recursivelyCloseSubItems()
        {
            for (int i = 0; i < getNumSubItems(); ++i)
            {
                getSubItem (i)->setOpen (false);
                dynamic_cast<ComponentTreeViewItem*> (getSubItem (i))->recursivelyCloseSubItems();
            }
        }

        void filterNodesRecursively (const juce::String& searchString)
        {
            // Iterate over the child nodes of the current node
            for (int i = getNumSubItems() - 1; i >= 0; --i)
            {
                // Recursively call the function on the current child node
                if (auto* ct = dynamic_cast<ComponentTreeViewItem*> (getSubItem (i)))
                {
                    ct->filterNodesRecursively (searchString);
                }
            }
            // Check if the current node's name does not contain the search string
            if (!getComponentName().containsIgnoreCase (searchString))
            {
                // Remove the subtree rooted at the current node
                if (getParentItem() != nullptr && getNumSubItems() == 0)
                {
                    getParentItem()->removeSubItem (getIndexInParent());
                }
                else
                    setOpen (true);
            }
            else if (getComponentName().startsWithIgnoreCase (searchString))
            {
                outlineComponentCallback (component);
                setSelected (true, true);
                setOpen (true);
            }
            else
            {
                setOpen (true);
            }
        }

        // Callback from the component listener. Reconstruct children when component is deleted
        void componentChildrenChanged (juce::Component& /*changedComponent*/) override
        {
            validateSubItems();
        }

        juce::String getComponentName()
        {
            juce::String res = "";
            if (component && !component->getName().isEmpty())
            {
                return component->getName();
            }
            else if (component)
            {
                return type (*component);
            }
            return res;
        }

        int getItemHeight() const override
        {
            return 29;
        }

        std::function<void (juce::Component* c)> outlineComponentCallback;
        std::function<void (juce::Component* c)> selectComponentCallback;

    private:
        juce::Component::SafePointer<juce::Component> component;
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ComponentTreeViewItem)

        void recursivelyAddChildrenFor (juce::Component* child)
        {
            // Components such as Labels can have a nullptr component child
            // Rather than display empty placeholders in the tree view, we will hide them
            if (child)
                addSubItem (new ComponentTreeViewItem (child, outlineComponentCallback, selectComponentCallback));
        }

        void addItemsForChildComponents()
        {
            for (int i = 0; i < component->getNumChildComponents(); ++i)
            {
                auto child = component->getChildComponent (i);
                if (componentString (child) != "Melatonin Overlay")
                    recursivelyAddChildrenFor (child);
            }
        }

        void validateSubItems()
        {
            for (int i = 0; i < getNumSubItems(); ++i)
            {
                // Ideally we'd just re-render the sub-items branch:
                // auto subItemToValidate = dynamic_cast<ComponentTreeViewItem*> (getSubItem (i));

                // However, that wasn't working so the scorched earth strategy is
                // if any child has a deleted component, we re-render the whole branch
                // (we don't explicitly know if things were added or removed)
                clearSubItems();
                addItemsForChildComponents();
                break;
            }
        }

        void selectTabbedComponentChildIfNeeded()
        {
            if (!getParentItem())
                return;

            auto parent = dynamic_cast<ComponentTreeViewItem*> (getParentItem());
            if (parent->hasTabbedComponent && !component->isVisible())
            {
                dynamic_cast<juce::TabbedComponent*> (parent->component.getComponent())->setCurrentTabIndex (getIndexInParent());
            }
        }
    };
}
