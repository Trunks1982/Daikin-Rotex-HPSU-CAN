class HPSUDashboardCard extends HTMLElement {
    setConfig(config) {
        if (!config.image) {
            throw new Error("Required image field is missing");
        }
        if (!config.entities) {
            throw new Error("Required entities field is missing");
        }

        this.entities_configuration = [
            { confEntityId: "temperature_outside", rectId: "T-Aussen-1-Value" },
            { confEntityId: "t_ext", rectId: "T-Aussen-2-Value" },
            { confEntityId: "expansion_valve", rectId: "EEV-Value" },
            { confEntityId: "t_liquid", rectId: "Kondensator-Value" },
            { confEntityId: "circulation_pump", rectId: "Umwaelzpumpe-Value" },
            { confEntityId: "flow_rate", rectId: "Durchfluss-Value" },
            { confEntityId: "tr", rectId: "Ruecklauf-1-Value" },
            { confEntityId: "inlet_water_temp_r4t", rectId: "Ruecklauf-2-Value" },
            { confEntityId: "heat_exchanger_mid_temp", rectId: "Verdampfer-Value" },
            { confEntityId: "tv", rectId: "Vorlauf-1-Value" },
            { confEntityId: "leaving_water_temp_before_buh", rectId: "Vorlauf-2-Value" },
            { confEntityId: "target_supply_temperature", rectId: "Vorlauf-Soll-Value" },
            { confEntityId: "water_pressure", rectId: "Druck-Value" },
            { confEntityId: "leaving_water_temp_after_buh", rectId: "Vorlauf-BH-1-Value" },
            { confEntityId: "tvbh", rectId: "Vorlauf-BH-2-Value" },
            { confEntityId: "fan_speed", rectId: "Luefter-Value" },
            { confEntityId: "inv_frequency_rps", rectId: "Verdichter-Value" },
            { confEntityId: "tdhw1", rectId: "Speicher-Value" },
            { confEntityId: "target_hot_water_temperature", rectId: "Speicher-Soll-Value" },
            { confEntityId: "dhw_mixer_position", rectId: "DHW-Mixer-Value", fontSize: "40px" },
            { confEntityId: "bypass_valve", rectId: "Bypass-Value", fontSize: "40px" }
        ];

        this.entities_configuration.forEach(entityConfig => {
            if (!config.entities[entityConfig.confEntityId]) {
                throw new Error("Required entity field is missing: " + entityConfig.confEntityId);
            }
        });

        console.log("aaaaaaaa");
  
        this.config = config;
        this.attachShadow({ mode: "open" });
        this.render();
    }
  
    async render() {
        const response = await fetch(this.config.image)
        if (!response.ok) {
            throw new Error(`HTTP error! status: ${response.status}`);
        }
        const svgContent = await response.text();
        
        const parser = new DOMParser();
        const svgDoc = parser.parseFromString(svgContent, "image/svg+xml");

        if (svgDoc.getElementsByTagName('parsererror').length > 0) {
            console.error("Fehler beim Parsen des SVG-Dokuments.");
            return;
        }

        const svgElement = svgDoc.documentElement;

        if (!svgElement.hasAttribute('viewBox')) {
            console.warn("Das SVG hat kein viewBox-Attribut gesetzt. Bitte setze es manuell.");
            svgElement.setAttribute("viewBox", "0 0 2897 1855");
        }

        svgElement.setAttribute("preserveAspectRatio", "xMidYMid meet");
        svgElement.setAttribute("width", "100%");
        svgElement.setAttribute("height", "100%");
        svgElement.style.width = "100%";
        svgElement.style.height = "100%";
        svgElement.style.display = "block";

        this.shadowRoot.innerHTML = "";

        const wrapper = document.createElement("div");
        wrapper.style.width = "100%";
        wrapper.style.height = "100vh";
        wrapper.style.overflow = "hidden";
        wrapper.style.display = "flex";
        wrapper.style.alignItems = "center";
        wrapper.style.justifyContent = "center";

        wrapper.style.minWidth = "0"; 
        wrapper.style.minHeight = "0";

        this.createStateLabels(svgDoc);

        wrapper.appendChild(svgElement);
        this.shadowRoot.appendChild(wrapper);


        this.updateOpacity();
    }
  
    updateOpacity() {
        const flowArrows = this.shadowRoot.querySelector(`#DHW-Flow-Arrows`);
        const flowReturnArrows = this.shadowRoot.querySelector(`#DHW-Flow-Return-Arrows`);
        const heatingArrows = this.shadowRoot.querySelector(`#Heating-Flow-Arrows`);

        if (!flowArrows || !flowReturnArrows || !heatingArrows) return;
  
        const mischerState = parseFloat(this._hass.states["sensor.rotex_dhw_mischer_position"].state);
        const bpvState = parseFloat(this._hass.states["sensor.rotex_bpv"].state);
        
        flowArrows.style.opacity = mischerState / 100.0;
        flowReturnArrows.style.opacity = (100 - bpvState) / 100.0;
        heatingArrows.style.opacity = bpvState / 100.0;
    }
  
    set hass(hass) {
      this._hass = hass;
      this.updateLabels();
      this.updateOpacity();
    }
  
    get hass() {
      return this._hass;
    }

    createStateLabels(svgDoc) {
        this.entities_configuration.forEach(state => {
            const valueBox = svgDoc.getElementById(state.rectId);
            if (valueBox) {
                const group = valueBox.parentNode;
                if (group && valueBox) {
                    const transform = valueBox.getAttribute('transform');
                    const fontSize = state.fontSize || "60px";

                    state.entityId = this.config.entities[state.confEntityId];

                    if (state.entityId) {
                        const label = document.createElementNS("http://www.w3.org/2000/svg", "text");
                        label.id = state.entityId.replace(".", "_");
                        label.setAttribute("text-anchor", "middle");
                        label.setAttribute("dominant-baseline", "middle");
                        label.setAttribute("cursor", "pointer");
                        label.setAttribute("fill", "blue");
                        label.setAttribute("font-size", fontSize);
                        label.setAttribute("fill", "silver");
                        if (transform) {
                            label.setAttribute("transform", transform);
                        }

                        console.log(state.confEntityId + "|" + state.entityId);

                        state.label = label;
                        state.valueBox = valueBox;

                        if (this._hass.states[state.entityId] !== undefined) {
                            label.textContent = this._hass.states[state.entityId].state;

                            console.log(label.textContent);
        
                            label.addEventListener("click", () => {
                                console.log("entityId: " + state.entityId);
                                this.handleStateClick(state.entityId);
                            });
            
                            group.appendChild(label);
                        } else {
                            log.warn(`Entity with ID ${state.entityId} not found`);
                        }
                    }
                } else {
                    console.warn(`Rect with ID ${state.rectId} not found.`);
                }
            } else {
                console.warn(`Rect with ID ${state.rectId} not found.`);
            }
        });

        this.updateLabels();
    }

    updateLabels() {
        if (this.entities_configuration) {
            this.entities_configuration.forEach(state => {
                if (state.entityId) {
                    console.log(state.entityId);
                    const newState = this._hass.states[state.entityId] || "";
                    if (state.valueBox) {
                        if (state.label) {
                            const entityState = newState.state || "--";
                            const unit = newState.attributes.unit_of_measurement || "";
                            state.label.textContent = `${entityState} ${unit}`;
    
                            const xPos = parseFloat(state.valueBox.getAttribute('x'));
                            const yPos = parseFloat(state.valueBox.getAttribute('y'));
                            const width = parseFloat(state.valueBox.getAttribute('width'));
                            const height = parseFloat(state.valueBox.getAttribute('height'));
    
                            state.label.setAttribute("x", xPos + width / 2);
                            state.label.setAttribute("y", yPos + height / 2 + 5);
                        } else {
                            console.warn("Label not found: " + state.entityId);
                        }
                    } else {
                        console.warn("ValueBox not found: " + state.entityId);
                    }
                }
            });
        }
    }

    handleStateClick(entityId) {
        const event = new CustomEvent('hass-more-info', {
            detail: {
                entityId: entityId
            },
            bubbles: true,
            composed: true
        });
        this.dispatchEvent(event);
    }
  }
  
  customElements.define("hpsu-dashboard-card", HPSUDashboardCard);